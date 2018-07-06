#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>

namespace msg {
    class Line {
    public:
        Line() = default;
        Line(glm::vec3 start, glm::vec3 end, float width = 1);

        inline void setBegin(std::shared_ptr<glm::vec3> &b) { begin = b; }
        inline void setEnd(std::shared_ptr<glm::vec3> &e) { end = e; }
        inline void setWidth(std::shared_ptr<float> &w) { width = w; }

        inline std::shared_ptr<glm::vec3> &getBegin() { return begin; }
        inline std::shared_ptr<glm::vec3> &getEnd() { return end; }
        inline std::shared_ptr<float> &getWidth() { return width; }

    public: // const
        inline glm::vec3 getDirection() const { return glm::normalize(*end - *begin); }
        inline virtual float getLength() const { return glm::distance(*begin, *end); }
        inline glm::vec3 getBegin() const { return *begin; }
        inline glm::vec3 getEnd() const { return *end; }
        inline float getWidth() const { return *width; }

    protected:
        std::shared_ptr<glm::vec3> begin;
        std::shared_ptr<glm::vec3> end;
        std::shared_ptr<float> width;
    };
}
