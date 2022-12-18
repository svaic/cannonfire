//
// Created by ljovanov on 05.11.2022.
//

#ifndef OPENGLPRJ_MOVABLEOBJ_HPP
#define OPENGLPRJ_MOVABLEOBJ_HPP


#include "RandomUtility.hpp"
#include "Shape.hpp"
#include "ShapeContainer.hpp"

class MovableObject : public ShapeContainer {
private:
    float speed;
    bool left;
    int iteration;
public:
    float x;
    float y;
    float width;
    float height;
    float health;
    float arsenal;

    MovableObject(float initX, float initY, float initSpeed, float initWidth = 1, float initHeight = 1);
    void moveRandomX(bool changeSide);
    void moveX(float offset);
    void moveY(float offset = -1);
    void changeSpeed(float newSpeed);
    void changeWidth(float newWidth);
    void changeSide();
    bool collide(MovableObject & other);
    void reduceHealth();
    void shoot();
    bool canShoot();
    void draw();
};


#endif //OPENGLPRJ_MOVABLEOBJ_HPP
