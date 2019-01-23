#pragma once 

#include <memory>
#include <glm/vec3.hpp>

namespace msg {
    class PulseWave {
        public:
            explicit PulseWave(float t, glm::vec3 o) :
                t(std::make_shared<float>(t)),
                origin(std::make_shared<glm::vec3>(o))
            {}

            inline std::shared_ptr<float> getTime() { return t; }

        public: // const
            inline float getTime() const { return *t; }
            inline glm::vec3 getOrigin() const { return *origin; }

        protected:
            std::shared_ptr<float> t;
            std::shared_ptr<glm::vec3> origin;
    };
}

