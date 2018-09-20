#pragma once 

#include "../geometry/Sphere.h"
#include "../geometry/SphereFactory.h"
#include <glm/mat4x4.hpp> // TODO fwd dec
#include <vector>
#include <memory>

namespace msg {
    class PulseWave;
    class Intersection;
}

namespace msg {
    class Shield : public msg::Sphere {
        public:
            Shield(glm::vec3 origin, float r, std::shared_ptr<glm::mat4> matrix);

            inline glm::mat4 getMatrix() { return *matrix; }
            void addIntersection(msg::Intersection &i); 
            void addPulseWave(msg::PulseWave &pw);     

            std::shared_ptr<std::vector<msg::Intersection>> intersections;
            std::shared_ptr<std::vector<msg::PulseWave>> pulseWaves;
            std::shared_ptr<glm::mat4> matrix;

    };
}
