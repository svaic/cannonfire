#ifndef OPENGLPRJ_OBSTACLE_HPP
#define OPENGLPRJ_OBSTACLE_HPP


#include "MovableObj.hpp"
#include "Color.hpp"
#include "Rectangle.hpp"

class Obstacle : public MovableObject {
private:
    bool canBeDestroyed;
    void changeColor(glm::vec3 color);
public:
    Obstacle();
    void move();
    bool reduceHealth();
};


#endif //OPENGLPRJ_OBSTACLE_HPP
