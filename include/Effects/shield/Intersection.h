#pragma once

#include <glm/vec3.hpp>
#include <chrono>
#include <memory>
#include <Effects/Effects_export.h>

namespace msg {
    class EFFECTS_EXPORT Intersection {
        public:
            explicit Intersection(glm::vec3 p) :
                point(p),
                t(std::make_shared<float>(0.0f))
            {}

            inline std::shared_ptr<float> &time() { return t; }
            inline bool deleteFlag() { return (*t == 1.0f); }

        public: // const
            inline glm::vec3 getOrigin() const { return point; }
            inline float getTime() const { return *t; }

        protected:
            glm::vec3 point;
            std::shared_ptr<float> t;
    };
}
