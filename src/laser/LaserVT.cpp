#include <Effects/laser/LaserVT.h>
#include <Effects/laser/Laser.h>

#include <iostream>
#include <vector>
#include <algorithm>

#include <geGL/geGL.h>
#include <geGL/OpenGLContext.h>
#include <geUtil/OrbitCamera.h>
#include <geUtil/PerspectiveCamera.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

msg::LaserVT::LaserVT(
    std::shared_ptr<ge::gl::Context> context,
    std::shared_ptr<ge::gl::Program> program,
    std::shared_ptr<ge::util::OrbitCamera> orbitCamera,
    std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera
) 
 : gl(context)
 , program(program)
 , orbitCamera(orbitCamera)
 , perspectiveCamera(perspectiveCamera)
 , verticies(std::make_shared<ge::gl::Buffer>(gl->getFunctionTable(), 256))
 , VAO(std::make_unique<ge::gl::VertexArray>(gl->getFunctionTable()))
 , OW(glm::lookAt(glm::vec3(0), glm::vec3(0,0,-5), glm::vec3(0,1,0)))
{
    VAO->addAttrib(verticies, 0, 3, GL_FLOAT, 0);
}

void msg::LaserVT::draw() {
    // std::cout << "LaserVT draw()" << std::endl;
	gl->glEnable(GL_BLEND);
    gl->glEnable(GL_BLEND);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    gl->glDepthMask(GL_FALSE);

    std::vector<glm::mat4> matrices = getLaserMatrices();
    ge::gl::Buffer modelMatrices(gl->getFunctionTable(), sizeof(glm::mat4) * matrices.size(), matrices.data());
    modelMatrices.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    
    program->use();
    texture->bind(0);
    VAO->bind();

    gl->glDrawArrays(GL_LINES, 0,static_cast<GLsizei>(lasers->size()*2));

    gl->glDepthMask(GL_TRUE);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl->glDisable(GL_BLEND);
}

void msg::LaserVT::update() {
    // std::cout << "LaserVT update()" << std::endl;
    
    auto cameraPosition = glm::vec3(glm::inverse(orbitCamera->getView())[3]);
    updateUniforms(cameraPosition);

    std::vector<glm::vec3> vertices;
    for(const auto &l : *lasers) {
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

    // check for reallocation
    auto dataByteSize = sizeof(glm::vec3) * vertices.size();
    auto bufferByteSize = verticies->getSize();
    if (bufferByteSize < dataByteSize) {
        verticies->realloc(bufferByteSize*2);
    }
    verticies->setData(vertices);
}

void msg::LaserVT::updateUniforms(const glm::vec3 &cameraPosition) {
    glm::vec4 WP(-viewport->x * 0.5f, -viewport->y * 0.5f, viewport->x, viewport->y);
    glm::mat4 OP = glm::ortho(-viewport->x * 0.5f, viewport->x * 0.5f, -viewport->y * 0.5f, viewport->y * 0.5f, -1.0f, 1.0f);

    program->set4fv("viewport", glm::value_ptr(WP));
    program->set3fv("_cameraPosition", glm::value_ptr(cameraPosition));
    program->setMatrix4fv("u_View", glm::value_ptr(OW));
    program->setMatrix4fv("u_Projection", glm::value_ptr(OP));
    program->setMatrix4fv("projectionMatrix", glm::value_ptr(perspectiveCamera->getProjection()));
    program->setMatrix4fv("viewMatrix", glm::value_ptr(orbitCamera->getView()));
}

std::vector<glm::mat4> msg::LaserVT::getLaserMatrices() {
    std::vector<glm::mat4> matrices;
    for(const std::shared_ptr<const msg::Laser> &laser : *lasers) {
        matrices.emplace_back(laser->matrix());
    }
    return matrices;
}