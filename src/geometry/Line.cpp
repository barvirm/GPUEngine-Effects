#include <Effects/geometry/Line.h>

msg::Line::Line(glm::vec3 start, glm::vec3 end, float width) :
    _begin(std::make_shared<glm::vec3>(start)),
    _end(std::make_shared<glm::vec3>(end)),
    _width(std::make_shared<float>(width))
{}
