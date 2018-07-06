#pragma once

#include <geometry/Line.h>
#include <memory>
#include <glm/glm.hpp>

namespace msg {
    class Laser : public msg::Line { 
    public:
        Laser() = default;
        Laser(glm::vec3 begin, glm::vec3 end, glm::vec4 color, float width);

        inline std::shared_ptr<glm::mat4> &getMatrix() { return matrix; }
        inline std::shared_ptr<glm::vec4> &getColor() { return color; }

    public: // const
        inline glm::vec3 getBegin() const { return *begin; }
        inline glm::vec3 getEnd() const { return *end; }
        inline float getWidth() const { return *width; }
        inline glm::vec4 getColor() const { return *color; }

        inline float getLength() const  { return glm::distance(*end, *begin); }
        inline glm::vec3 getDirection() const { return glm::normalize(*end - *begin); }
 

    protected:
        std::shared_ptr<glm::vec4> color;
        std::shared_ptr<glm::mat4> matrix;
    };
}

