#pragma once

#include "../VisualizationTechnique.h"
#include "LaserVT.h"

#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


namespace ge {
    namespace gl {
        class VertexArray;
        class Context;
        class Program;
        class Texture;
    }
    namespace util {
        class PerspectiveCamera;
        class OrbitCamera;
    }
}
namespace msg {
    class Laser;
}

namespace msg {
    class LaserVT : public VisualizationTechnique {
        public:
            LaserVT() = default;
            void init(std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera, std::shared_ptr<ge::util::OrbitCamera> orbitCamera);
            void updateViewport(float w, float h);
            void draw() override;
            void update() override;

            std::shared_ptr<ge::gl::Context> gl;
            std::shared_ptr<ge::gl::Program> program;

            std::shared_ptr<std::vector<std::shared_ptr<msg::Laser>>> lasers;
            std::shared_ptr<ge::gl::Texture> texture;
        protected:
           void updateCameraPosition();
           std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera;
           std::shared_ptr<ge::util::OrbitCamera> orbitCamera;

           glm::mat4 _orthoProjection;
           glm::mat4 _orthoView;
           glm::vec3 _cameraPosition;
           glm::vec4 _viewport;

        private:
            void CreateVAO();
            void CalculateLaser(const std::shared_ptr<msg::Laser> &laser);
            void Clear();

            std::vector<unsigned int> _indicies;
            std::vector<glm::vec2> _vertices;
            std::vector<float> _texCoords;
            std::vector<float> _colors;
            std::vector<float> _depths;

            std::shared_ptr<ge::gl::VertexArray> _VAO;
    };
}

