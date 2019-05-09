#pragma once

#include <Effects/VisualizationTechnique.h>

#include <memory>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <Effects/Effects_export.h>





namespace ge {
    namespace gl {
        class VertexArray;
        class Context;
        class Program;
        class Texture;
        class Buffer;
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
    class EFFECTS_EXPORT LaserVT : public VisualizationTechnique {
        public:
            LaserVT(
                std::shared_ptr<ge::gl::Context> context,
                std::shared_ptr<ge::gl::Program> program,
                std::shared_ptr<ge::util::OrbitCamera> orbitCamera,
                std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera
            );
            ~LaserVT() = default;
            virtual void draw() override;
            virtual void update() override;

            std::shared_ptr<std::vector<std::shared_ptr<msg::Laser>>> lasers;
            std::shared_ptr<ge::gl::Texture> texture;
            
            std::shared_ptr<glm::vec2> viewport;
        protected:
            std::shared_ptr<ge::gl::Context> gl;
            std::shared_ptr<ge::gl::Program> program;

            std::shared_ptr<ge::util::OrbitCamera> orbitCamera;
            std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera;

            std::unique_ptr<ge::gl::VertexArray> VAO;
            std::shared_ptr<ge::gl::Buffer> verticies;

            std::vector<glm::mat4> getLaserMatrices();
            void updateUniforms(const glm::vec3 &cameraPosition);

            const glm::mat4 OW;
    };
}

