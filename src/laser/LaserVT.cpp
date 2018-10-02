#include "LaserVT.h"
#include <iostream>
#include <geGL/geGL.h>
#include <geUtil/OrbitCamera.h>
#include <geUtil/PerspectiveCamera.h>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <algorithm>
#include "Laser.h"

void msg::LaserVT::draw() {
    std::cout << "LaserVT draw()" << std::endl;
    gl->glEnable(GL_BLEND);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    gl->glDepthMask(GL_FALSE);

    std::vector<glm::mat4> matrices;
    for(const std::shared_ptr<const msg::Laser> &l : *lasers) {
        matrices.emplace_back(l->matrix());
    }
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
    std::cout << "LVT End" << std::endl;
}

void msg::LaserVT::update() {
    //std::cout << "LaserVT update()" << std::endl;
    _VAO = new ge::gl::VertexArray(gl->getFunctionTable());
    auto cameraPosition = glm::vec3(glm::inverse(orbitCamera->getView())[3]);
    updateUniforms(cameraPosition);

    std::vector<glm::vec3> vertices;
    for(const std::shared_ptr<const msg::Laser> &l : *lasers) {
        glm::vec3 b(l->begin());
        glm::vec3 e(l->end());

        float bl = glm::length(b - cameraPosition);
        float el = glm::length(e - cameraPosition);

        if (bl > el) {
            vertices.emplace_back(l->begin());
            vertices.emplace_back(l->end());
        }
        else {
            vertices.emplace_back(l->end());
            vertices.emplace_back(l->begin());
        }
    }
    auto FT = gl->getFunctionTable();
    auto vert(std::make_shared<ge::gl::Buffer>(FT, sizeof(glm::vec3) * vertices.size(), vertices.data()));
    _VAO->addAttrib(vert, 0, 3, GL_FLOAT, 0);
}

void msg::LaserVT::updateUniforms(const glm::vec3 &cameraPosition) {
    glm::vec4 WP(-viewport->x * 0.5f, -viewport->y * 0.5f, viewport->x, viewport->y);
    glm::mat4 OP = glm::ortho(-viewport->x * 0.5f, viewport->x * 0.5f, -viewport->y * 0.5f, viewport->y * 0.5f, -1.0f, 1.0f);
    const glm::mat4 OW = glm::lookAt(glm::vec3(0), glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0));

    program->set4fv("viewport", glm::value_ptr(WP));
    program->set3fv("_cameraPosition", glm::value_ptr(cameraPosition));
    program->setMatrix4fv("u_View", glm::value_ptr(OW));
    program->setMatrix4fv("u_Projection", glm::value_ptr(OP));
    program->setMatrix4fv("projectionMatrix", glm::value_ptr(perspectiveCamera->getProjection()));
    program->setMatrix4fv("viewMatrix", glm::value_ptr(orbitCamera->getView()));
}
