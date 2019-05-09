#pragma once


#include <geSG/BoundingSphere.h>
#include <glm/vec3.hpp>
#include <Effects/Effects_export.h>

namespace msg {
    class EFFECTS_EXPORT Sphere : public ge::sg::BoundingSphere {
        public:

            Sphere() = default;
            Sphere(glm::vec3 origin, float r);

            virtual inline float getRadius() const { return radius; }
            virtual inline glm::vec3 getPosition() const { return center; }

            virtual inline void setRadius(float r) { radius = r; }
            virtual inline void setPosition(glm::vec3 p) { center = p; }

    };
}
