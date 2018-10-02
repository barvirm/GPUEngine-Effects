#pragma once

#include "../geometry/Line.h"
#include <memory>
#include <glm/glm.hpp>

namespace msg {
    class Laser : public msg::Line { 
    public:
        Laser() = default;
        Laser(glm::vec3 begin, glm::vec3 end, glm::vec4 color, float width);

        inline std::shared_ptr<glm::mat4> &matrix() { return _matrix; }
        inline std::shared_ptr<glm::vec4> &color() { return _color; }

    public: // const
        inline glm::vec4 color() const { return *_color; }
        inline glm::mat4 matrix() const { return *_matrix; }

    protected:
        std::shared_ptr<glm::vec4> _color;
        std::shared_ptr<glm::mat4> _matrix;
    };
}

