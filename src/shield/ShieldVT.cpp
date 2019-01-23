#include <Effects/shield/ShieldVT.h>
#include <Effects/shield/Shield.h>
#include <Effects/shield/PulseWave.h>
#include <Effects/shield/Intersection.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <geGL/geGL.h>

#include <geUtil/OrbitCamera.h>
#include <geUtil/PerspectiveCamera.h>

void msg::ShieldVT::draw() {
    //std::cout << "ShieldVT draw" << std::endl;
    gl->glEnable(GL_BLEND);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    gl->glDepthMask(GL_FALSE);

    std::vector<glm::mat4> matricies;
    std::vector<glm::vec4> intersections;
    std::vector<glm::vec4> pulseWaves;
    std::vector<int> ps_intersection;
    std::vector<int> ps_pulseWave;

    auto addPulseWave = [&pulseWaves](const msg::PulseWave &p) {
        pulseWaves.emplace_back(p.getOrigin(), p.getTime());
    };

    auto addIntersection = [&intersections](const msg::Intersection &i) {
        intersections.emplace_back(i.getOrigin(), i.getTime());
    };

    for (Shield &s : *shields) {
        ps_pulseWave.emplace_back(s.pulseWaves->size());
        ps_intersection.emplace_back(s.intersections->size());
        matricies.emplace_back(s.getMatrix());
        std::for_each(s.pulseWaves->begin(), s.pulseWaves->end(), addPulseWave);
        std::for_each(s.intersections->begin(), s.intersections->end(), addIntersection);
    }

    std::partial_sum(ps_intersection.begin(), ps_intersection.end(), ps_intersection.begin());
    std::partial_sum(ps_pulseWave.begin(), ps_pulseWave.end(), ps_pulseWave.begin());

    ps_intersection.insert(ps_intersection.begin(), 0);
    ps_pulseWave.insert(ps_pulseWave.begin(), 0);


    program->use();
    _VAO->bind();

    auto FT = gl->getFunctionTable();
    ge::gl::Buffer PulseWaveInfo(FT, sizeof(glm::vec4) * pulseWaves.size(), pulseWaves.data());
    ge::gl::Buffer PrefixSumIntersectons(FT, sizeof(int) * ps_intersection.size(), ps_intersection.data());
    ge::gl::Buffer IntersectionPoints(FT, sizeof(glm::vec4) * intersections.size(), intersections.data());
    ge::gl::Buffer PrefixSumPulseWaves(FT, sizeof(int) * ps_pulseWave.size(), ps_pulseWave.data());
    ge::gl::Buffer ModelMatricies(FT, sizeof(glm::mat4) * matricies.size(), matricies.data());

    PulseWaveInfo.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    PrefixSumIntersectons.bindBase(GL_SHADER_STORAGE_BUFFER, 1);
    IntersectionPoints.bindBase(GL_SHADER_STORAGE_BUFFER, 2);
    PrefixSumPulseWaves.bindBase(GL_SHADER_STORAGE_BUFFER, 3);
    ModelMatricies.bindBase(GL_SHADER_STORAGE_BUFFER, 4);

    gl->glDrawArraysInstanced(
        GL_TRIANGLE_STRIP, 
        0, 
        static_cast<GLsizei>(_sphereVertex.size() / 3 ),
        static_cast<GLsizei>(shields->size())
    );

    gl->glDepthMask(GL_TRUE);
    gl->glDisable(GL_BLEND);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void msg::ShieldVT::update() {
    //std::cout << "ShieldVT update" << std::endl;
    program->setMatrix4fv("projectionMatrix", glm::value_ptr(perspectiveCamera->getProjection()));
    program->setMatrix4fv("viewMatrix", glm::value_ptr(orbitCamera->getView()));
    program->set1f("time", *time);
}

void msg::ShieldVT::init() {
    std::cout << "ShieldVT init" << std::endl;
    auto FT = gl->getFunctionTable();
    _VAO = std::make_shared<ge::gl::VertexArray>(FT);
    _sphereVertex = SphereFactory::create(20);
    auto vert(std::make_shared<ge::gl::Buffer>(FT, sizeof(float) * _sphereVertex.size(), _sphereVertex.data()));
    _VAO->addAttrib(vert, static_cast<GLuint>(0), 3, GL_FLOAT);
}
