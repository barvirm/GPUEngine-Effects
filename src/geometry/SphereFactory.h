#pragma once

#include <vector>

namespace msg {
    class SphereFactory {
    public:
        static std::vector<float> create(const unsigned &grad) {
            std::vector<float> vertices;
            const float pi = 3.141592f;
            for (float a = 0.0f; a < pi; a += pi / grad) {
                for (float b = 0.0f; b < 2.0f * pi; b += pi / grad) {
                    vertices.push_back(cos(b) * sin(a));
                    vertices.push_back(sin(b) * sin(a));
                    vertices.push_back(cos(a));
                    vertices.push_back(cos(b) * sin(a + pi / grad));
                    vertices.push_back(sin(b) * sin(a + pi / grad));
                    vertices.push_back(cos(a + pi / grad));
                }
            }
            return vertices;
        }
    };
}


