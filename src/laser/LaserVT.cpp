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

    glm::mat4 model(1);
    //program->setMatrix4fv("modelMatrix", glm::value_ptr(model));

    program->use();
    texture->bind(0);
    _VAO->bind();

    gl->glDrawArrays(GL_LINES, 0, 2);

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
    std::for_each(lasers->begin(), lasers->end(),
        [&vertices, &cameraPosition](const std::shared_ptr<msg::Laser> &l) {
            glm::vec3 b(l->getBegin());
            glm::vec3 e(l->getEnd());

            float bl = glm::length(b - cameraPosition);
            float el = glm::length(e - cameraPosition);

            if ( bl > el ) {
                vertices.emplace_back(l->getBegin());
                vertices.emplace_back(l->getEnd());
            }
            else {
                vertices.emplace_back(l->getBegin());
                vertices.emplace_back(l->getEnd());
            }

        }
    );
    /*
    std::vector<float> vertices;
    std::for_each(lasers->begin(), lasers->end(), 
        [&vertices, &cameraPosition](const std::shared_ptr<msg::Laser> &l) {
            glm::vec3 b(l->getBegin());
            glm::vec3 e(l->getEnd());

            float bl = glm::length(b - cameraPosition);
            float el = glm::length(b - cameraPosition);


        }
    );
        */
    
    _VAO->addAttrib(
        std::make_shared<ge::gl::Buffer>(gl->getFunctionTable(), sizeof(glm::vec3) * vertices.size(), vertices.data()),
        static_cast<GLuint>(0),
        3,
        GL_FLOAT,
        (GLsizei) 0
    );
}
