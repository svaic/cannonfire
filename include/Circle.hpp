#ifndef OPENGLPRJ_CIRCLE_HPP
#define OPENGLPRJ_CIRCLE_HPP


#include <vector>
#include "Shape.hpp"

class Circle : public Shape {
public:
    Circle(std::vector<float> vertices, glm::vec2 initialPos);
    void draw();
    static Circle createCircle(float radius, glm::vec2 initialPosition, glm::vec3 color);
};


#endif //OPENGLPRJ_CIRCLE_HPP
