#include "LaserVT.h"
#include "Laser.h"

#include <geGL/geGL.h>
#include <geUtil/OrbitCamera.h>
#include <geUtil/PerspectiveCamera.h>
#include <geSG/Material.h>
#include <algorithm>


void msg::LaserVT::update() {
    Clear();
    updateCameraPosition();
    std::for_each(lasers->begin(), lasers->end(), [this](const auto &laser) {CalculateLaser(laser);});
    CreateVAO();
}

void msg::LaserVT::CalculateLaser(const std::shared_ptr<msg::Laser> &laser) {
    const float DIST_SCALING_FACTOR = 1500.0f;
    const glm::vec2 EPS(0.01f);
    const float HEAD_ON_FALLOFF = 1.0f;

    glm::vec3 begin = laser->getBegin();
    glm::vec3 end = laser->getEnd();
    glm::mat4 viewMatrix = orbitCamera->getView();
    glm::mat4 projectionMatrix = perspectiveCamera->getProjection();

    glm::vec3 orthoP1 = project(begin, viewMatrix, projectionMatrix, _viewport);
    glm::vec3 orthoP2 = project(end, viewMatrix, projectionMatrix, _viewport);

    if (orthoP1.z < 1.0f || orthoP2.z < 1.0f) {

        float begin_df = DIST_SCALING_FACTOR / glm::length(begin - _cameraPosition);
        float end_df = DIST_SCALING_FACTOR / glm::length(begin - _cameraPosition);

        glm::vec2 pointDistance = glm::abs(orthoP1 - orthoP2);
        orthoP1.x += EPS.x * ( glm::any(glm::lessThan(pointDistance,EPS)) );

        orthoP1 *= -glm::sign(orthoP1.z - 1.0f);
        orthoP2 *= -glm::sign(orthoP2.z - 1.0f);

        glm::vec3 cameraLaserDirection = glm::normalize(_cameraPosition - ((begin + end) / 2.0f));
        float angle = glm::dot(laser->getDirection(), cameraLaserDirection);
        float headOnFactor = pow(abs(angle), HEAD_ON_FALLOFF);

        if (begin_df > end_df) {
            end_df = end_df + (begin_df - end_df) * headOnFactor;
        }
        else {
            begin_df = begin_df + (end_df - begin_df) *headOnFactor;
        }

        float beginWidth = begin_df * laser->getWidth();
        float endWidth = end_df * laser->getWidth();

        glm::vec2 direction = glm::normalize(glm::vec2(orthoP1 - orthoP2));

        glm::vec2 beginY = direction * beginWidth;
        glm::vec2 beginX = glm::vec2(beginY.y, -begin.x);
        glm::vec2 endY = direction * endWidth;
        glm::vec2 endX = glm::vec2(endY.y, -endY.x);

        _vertices.emplace_back(glm::vec2(orthoP1) - beginX + beginY);
        _vertices.emplace_back(glm::vec2(orthoP1) + beginX + beginY);
        _vertices.emplace_back(glm::vec2(orthoP1) - beginX);
        _vertices.emplace_back(glm::vec2(orthoP1) + beginX);
        _vertices.emplace_back(glm::vec2(orthoP2) - endX);
        _vertices.emplace_back(glm::vec2(orthoP2) + endX);
        _vertices.emplace_back(glm::vec2(orthoP2) - endX - endY);
        _vertices.emplace_back(glm::vec2(orthoP2) + endX - endY);

        std::array<float, 24> tc{
        0.0f, beginWidth, beginWidth, 
        beginWidth, beginWidth, beginWidth,
        0.0f, beginWidth*0.5f, beginWidth,
        beginWidth, beginWidth*0.5f, beginWidth,

        0.0f, endWidth*0.5f, endWidth,
        endWidth, endWidth*0.5f, endWidth,
        0.0f, 0.0f, endWidth,
        endWidth, 0.0f, endWidth
        };

        _texCoords.insert(_texCoords.end(), tc.begin(), tc.end());

        glm::vec4 color = *laser->getColor();
        for (int i = 0; i < 8; i++) {
            _colors.push_back(color.r);
            _colors.push_back(color.g);
            _colors.push_back(color.b);
            _colors.push_back(color.a);
        }

        for (int i = 0; i < 4; i++) {
            _depths.push_back(orthoP1.z);
        }
        for (int i = 0; i < 4; i++) {
            _depths.push_back(orthoP2.z);
        }
    }
}

void msg::LaserVT::CreateVAO() { 
    for(auto i = 0; i < lasers->size(); i++) {
        std::array<int,18> ind {2,0,1,1,3,2,4,2,3,3,5,4,6,4,5,5,7,6};
        std::transform(ind.begin(), ind.end(), ind.begin(), [&i](int a) -> int { return a+(8*i);});
    }

    _VAO = std::make_shared<ge::gl::VertexArray>(gl->getFunctionTable());
    _VAO->addElementBuffer(std::make_shared<ge::gl::Buffer>(sizeof(unsigned) * _indicies.size(), _indicies.data()));
    _VAO->addAttrib(
            std::make_shared<ge::gl::Buffer>(sizeof(float) * _vertices.size(), &_vertices[0]),
            static_cast<GLuint>(0),
            2,
            GL_FLOAT,
            (GLsizei) 0
    );
    _VAO->addAttrib(
            std::make_shared<ge::gl::Buffer>(sizeof(float) * _texCoords.size(), &_texCoords[0]),
            static_cast<GLuint>(1),
            3,
            GL_FLOAT,
            (GLsizei) 0
    );
    _VAO->addAttrib(
            std::make_shared<ge::gl::Buffer>(sizeof(float) * _colors.size(), &_colors[0]),
            static_cast<GLuint>(2),
            4,
            GL_FLOAT,
            (GLsizei) 0
    );
    _VAO->addAttrib(
            std::make_shared<ge::gl::Buffer>(sizeof(float) * _depths.size(), &_depths[0]),
            static_cast<GLuint>(3),
            1,
            GL_FLOAT,
            (GLsizei) 0
    );
}

void msg::LaserVT::draw() {
    gl->glEnable(GL_BLEND);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    gl->glDepthMask(GL_FALSE);


    program->use();

    program->setMatrix4fv("u_View", glm::value_ptr(_orthoView));
    program->setMatrix4fv("u_Projection", glm::value_ptr(_orthoProjection));

    program->use();
    texture->bind(0);
    _VAO->bind();

    gl->glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(18 * (_depths.size() / 8)), GL_UNSIGNED_INT, nullptr);

    gl->glDepthMask(GL_TRUE);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl->glDisable(GL_BLEND);
}

void msg::LaserVT::Clear() {
    _VAO = nullptr;
    _indicies.clear();
    _vertices.clear();
    _texCoords.clear();
    _colors.clear();
    _depths.clear();
}

void msg::LaserVT::init(std::shared_ptr<ge::util::PerspectiveCamera> perspectivecamera, std::shared_ptr<ge::util::OrbitCamera> orbitcamera) {
    perspectiveCamera = perspectivecamera;
    orbitCamera = orbitcamera;
}

void msg::LaserVT::updateViewport(float w, float h) {
    _viewport = {-w * 0.5f, -h * 0.5f, w, h};
    _orthoProjection = glm::ortho(-w * 0.5f, w * 0.5f, -h * 0.5f, h / 2.0f, -1.0f, 1.0f);
    _orthoView = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0));
}

void msg::LaserVT::updateCameraPosition() {
    _cameraPosition = static_cast<glm::vec3>( glm::inverse(orbitCamera->getView())[3]);
}

