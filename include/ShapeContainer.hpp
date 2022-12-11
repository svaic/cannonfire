//
// Created by ljovanov on 06.11.2022.
//

#ifndef OPENGLPRJ_SHAPECONTAINER_HPP
#define OPENGLPRJ_SHAPECONTAINER_HPP


#include <vector>
#include "Shape.hpp"

class ShapeContainer {
protected:
    std::vector<Shape*> shapes;
protected:
    void draw();
    void transform(glm::vec2 move, glm::vec2 scale, float angle);
    void move(glm::vec2 move);
    void scale(glm::vec2 scale);
    void rotate(float angle);
public:
    static unsigned int shaderId;
    void add(Shape* shape);
    void remove(int i);

    static void setShaderId(unsigned int id);
};


#endif //OPENGLPRJ_SHAPECONTAINER_HPP
