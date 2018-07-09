#pragma once

#include "../VisualizationTechnique.h"

#include <memory>
#include <vector>
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
    }
}

namespace msg {
    class Laser;
}

namespace msg {
    class LaserVT : public eff::msg::VisualizationTechnique {
        public:
            LaserVT() = default;
            void draw() override;
            void update() override;

            std::shared_ptr<ge::gl::Context> gl;
            std::shared_ptr<ge::gl::Program> program;

            std::shared_ptr<std::vector<std::shared_ptr<msg::Laser>>> lasers;
            std::shared_ptr<ge::gl::Texture> texture;
            std::shared_ptr<ge::util::OrbitCamera> orbitCamera;
        private:
            std::vector<glm::vec3> _vertices;
            ge::gl::VertexArray *_VAO;
    };
}

