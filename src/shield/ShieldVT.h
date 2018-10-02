#pragma once 

#include "../VisualizationTechnique.h"
#include <memory>
#include <vector>

namespace ge {
    namespace gl {
        class Context;
        class Program;
        class VertexArray;
        class Buffer;
    }
}

namespace msg {
    class Shield;
}

namespace msg {
    class ShieldVT : public VisualizationTechnique {

        public:
            void init();
            void draw() override;
            void update() override;

            std::shared_ptr<ge::gl::Context> gl;
            std::shared_ptr<ge::gl::Program> program;
            std::shared_ptr<std::vector<msg::Shield>> shields;

        protected:
            std::vector<float> _sphereVertex;
        private:
            std::shared_ptr<ge::gl::VertexArray> _VAO;
    };
}
