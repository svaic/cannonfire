//
// Created by ljovanov on 05.11.2022.
//

#ifndef OPENGLPRJ_MOVABLEOBJ_HPP
#define OPENGLPRJ_MOVABLEOBJ_HPP


#include "RandomUtility.hpp"

class MovableObject {
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
    void moveRandom(bool changeSide);
    void move_x(float offset);
    void move_y();
    void changePosition();

    bool collide(MovableObject & other);
    void hit();
    void shoot();
    bool canShoot();
};


#endif //OPENGLPRJ_MOVABLEOBJ_HPP
