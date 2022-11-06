#ifndef OPENGLPRJ_CIRCLE_HPP
#define OPENGLPRJ_CIRCLE_HPP


#include <vector>
#include "Shape.hpp"

class Circle : public Shape {
public:
    Circle(std::vector<float> vertices);
    void draw();
    static Circle createCircle(float radius, glm::vec3 color);
};


#endif //OPENGLPRJ_CIRCLE_HPP
