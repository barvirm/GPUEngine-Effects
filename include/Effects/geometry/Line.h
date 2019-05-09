#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <Effects/Effects_export.h>

namespace msg {
    class EFFECTS_EXPORT Line {
    public:
        Line() = default;
        Line(glm::vec3 start, glm::vec3 end, float width = 1);

        inline void begin(std::shared_ptr<glm::vec3> &b) { _begin = b; }
        inline void end(std::shared_ptr<glm::vec3> &e) { _end = e; }
        inline void setWidth(std::shared_ptr<float> &w) { _width = w; }

        virtual inline std::shared_ptr<glm::vec3> &beginPtr() { return _begin; }
        virtual inline std::shared_ptr<glm::vec3> &endPtr() { return _end; }
        virtual inline std::shared_ptr<float> &widthPtr() { return _width; }

    public: // const
        inline glm::vec3 direction() const { return glm::normalize(*_end - *_begin); }
        inline float length() const { return glm::distance(*_begin, *_end); }
        inline glm::vec3 begin() const { return *_begin; }
        inline glm::vec3 end() const { return *_end; }
        inline float width() const { return *_width; }

    protected:
        std::shared_ptr<glm::vec3> _begin;
        std::shared_ptr<glm::vec3> _end;
        std::shared_ptr<float> _width;
    };
}
