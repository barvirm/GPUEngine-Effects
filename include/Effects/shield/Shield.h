#pragma once 

#include <Effects/geometry/Sphere.h>
#include <Effects/geometry/SphereFactory.h>
#include <glm/fwd.hpp>
#include <vector>
#include <memory>
#include <Effects/Effects_export.h>

namespace msg {
    class PulseWave;
    class Intersection;
}

namespace msg {
    class EFFECTS_EXPORT Shield : public msg::Sphere {
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
