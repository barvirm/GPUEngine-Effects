#include "Laser.h"

msg::Laser::Laser(glm::vec3 begin, glm::vec3 end, glm::vec4 color, float width) {
    _begin = std::make_shared<glm::vec3>(begin);
    _end = std::make_shared<glm::vec3>(end);
    _width = std::make_shared<float>(width);

    _color = std::make_shared<glm::vec4>(color);
    _matrix = std::make_shared<glm::mat4>(1);
}


