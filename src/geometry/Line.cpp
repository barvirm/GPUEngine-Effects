#include <geometry/Line.h>

msg::Line::Line(glm::vec3 start, glm::vec3 end, float width) :
    begin(std::make_shared<glm::vec3>(start)),
    end(std::make_shared<glm::vec3>(end)),
    width(std::make_shared<float>(width))
{}
