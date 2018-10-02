#pragma once

#include "../VisualizationTechnique.h"

#include <memory>
#include <vector>
//#include <glm/fwd.hpp>
#include <glm/vec3.hpp>



namespace ge {
    namespace gl {
        class VertexArray;
        class Context;
        class Program;
        class Texture;
    }
    namespace util {
        class OrbitCamera;
        class PerspectiveCamera;
    }
}

namespace msg {
    class Laser;
}

namespace msg {
    class LaserVT : public VisualizationTechnique {
        public:
            LaserVT() = default;
            ~LaserVT() = default;
            virtual void draw() override;
            virtual void update() override;

            std::shared_ptr<ge::gl::Context> gl;
            std::shared_ptr<ge::gl::Program> program;

            std::shared_ptr<std::vector<std::shared_ptr<msg::Laser>>> lasers;
            std::shared_ptr<ge::gl::Texture> texture;

            // update uniforms
            std::shared_ptr<ge::util::OrbitCamera> orbitCamera;
            std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera;
            std::shared_ptr<glm::vec2> viewport;
        private:
            std::vector<glm::vec3> _vertices;
            ge::gl::VertexArray *_VAO;

            void updateUniforms(const glm::vec3 &cameraPosition);
    };
}

