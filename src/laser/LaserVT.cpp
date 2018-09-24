#include "LaserVT.h"
#include <iostream>
#include <geGL/geGL.h>
#include <geUtil/OrbitCamera.h>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <algorithm>
#include "Laser.h"

void msg::LaserVT::draw() {
    //std::cout << "LaserVT draw()" << std::endl;
    gl->glEnable(GL_BLEND);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    gl->glDepthMask(GL_FALSE);

    std::vector<glm::mat4> matrices;
    std::for_each(lasers->begin(), lasers->end(), 
        [&matrices](const std::shared_ptr<const msg::Laser> &l) { 
            matrices.emplace_back(l->getMatrix()); 
        }
    );
    ge::gl::Buffer modelMatrices(gl->getFunctionTable(), sizeof(glm::mat4) * matrices.size(), matrices.data());
    modelMatrices.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    
    program->use();
    texture->bind(0);
    _VAO->bind();

    gl->glDrawArrays(GL_LINES, 0, lasers->size()*2);

    delete _VAO;
    gl->glDepthMask(GL_TRUE);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl->glDisable(GL_BLEND);
}

void msg::LaserVT::update() {
    //std::cout << "LaserVT update()" << std::endl;
    _VAO = new ge::gl::VertexArray(gl->getFunctionTable());
    auto cameraPosition = glm::vec3(glm::inverse(orbitCamera->getView())[3]);

    std::vector<glm::vec3> vertices;
    for(const std::shared_ptr<const msg::Laser> &l : *lasers) {
        glm::vec3 b(l->getBegin());
        glm::vec3 e(l->getEnd());

        float bl = glm::length(b - cameraPosition);
        float el = glm::length(e - cameraPosition);

        if (bl > el) {
            vertices.emplace_back(l->getBegin());
            vertices.emplace_back(l->getEnd());
        }
        else {
            vertices.emplace_back(l->getEnd());
            vertices.emplace_back(l->getBegin());
        }
    }
    auto FT = gl->getFunctionTable();
    auto vert(std::make_shared<ge::gl::Buffer>(FT, sizeof(glm::vec3) * vertices.size(), vertices.data()));
    _VAO->addAttrib(vert, 0, 3, GL_FLOAT, 0);
}
