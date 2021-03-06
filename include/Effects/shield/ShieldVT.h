#pragma once 

#include <Effects/VisualizationTechnique.h>
#include <memory>
#include <vector>
#include <Effects/Effects_export.h>

namespace ge {
    namespace gl {
        class Context;
        class Program;
        class VertexArray;
        class Buffer;
    }
    namespace util {
        class OrbitCamera;
        class PerspectiveCamera;
    }
}

namespace msg {
    class Shield;
}

namespace msg {
    class EFFECTS_EXPORT ShieldVT : public VisualizationTechnique {

        public:
            void init();
            void draw() override;
            void update() override;

            std::shared_ptr<ge::gl::Context> gl;
            std::shared_ptr<ge::gl::Program> program;
            std::shared_ptr<std::vector<msg::Shield>> shields;
            std::shared_ptr<ge::util::OrbitCamera> orbitCamera;
            std::shared_ptr<ge::util::PerspectiveCamera> perspectiveCamera;
            std::shared_ptr<double> time;

        protected:
            std::vector<float> _sphereVertex;
        private:
            std::shared_ptr<ge::gl::VertexArray> _VAO;
            std::shared_ptr<ge::gl::Buffer> vb;
    };
}
