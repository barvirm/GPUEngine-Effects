#include <Effects/shield/Shield.h>
#include <Effects/shield/Intersection.h>
#include <Effects/shield/PulseWave.h>
#include <iostream>
#include <algorithm>
#include <glm/mat4x4.hpp>

msg::Shield::Shield(glm::vec3 origin, float r, std::shared_ptr<glm::mat4> matrix = std::make_shared<glm::mat4>(1.0)) :
    msg::Sphere(origin, r),
    intersections(std::make_shared<std::vector<msg::Intersection>>()),
    pulseWaves(std::make_shared<std::vector<msg::PulseWave>>()),
    matrix(matrix)
    {}


void msg::Shield::addIntersection(msg::Intersection &i) { intersections->emplace_back(i); }
void msg::Shield::addPulseWave(msg::PulseWave &pw) { pulseWaves->emplace_back(pw); }
