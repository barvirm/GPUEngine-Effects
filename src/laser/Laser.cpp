#include "Laser.h"

msg::Laser::Laser(glm::vec3 begin, glm::vec3 end, glm::vec4 color, float width) {
    this->begin = std::make_shared<glm::vec3>(begin);
    this->end = std::make_shared<glm::vec3>(end);
    this->width = std::make_shared<float>(width);

    this->color = std::make_shared<glm::vec4>(color);
    this->matrix = std::make_shared<glm::mat4>(1);
}


