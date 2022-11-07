//
// Created by ljovanov on 05.11.2022.
//

#include "MovableObj.hpp"

MovableObject::MovableObject(float initX, float initY, float initSpeed, float initWidth, float initHeight) {
    x = initX;
    y = initY;
    left = true;
    speed = initSpeed;
    health = 1.0;
    width = initWidth;
    height = initHeight;
    iteration = 0;
    arsenal = 1;
}

void MovableObject::moveRandom(bool changeSide) {
    if (changeSide) changePosition();
    if (x - speed <= -1.00 && left == true) {
        left = false;
    }
    else if (x + speed >= 1.00 && left == false) {
        left = true;
    }
    if (left) {
        x = x - speed;
    } else {
        x = x + speed;
    }
}

void MovableObject::move_x(float offset) {
    x += offset;
}

void MovableObject::move_y() {
    y += speed;
}

void MovableObject::changePosition() {
    if (iteration == 10) {
        iteration = 0;

        speed = RandomUtility().getRandomFloat(0.05, 0.01);

        int shouldChangeDirectionCoinFlip = RandomUtility().getRandomInt(0,10);
        if (shouldChangeDirectionCoinFlip > 7)
            left = !left;
    }
    iteration++;
}

bool MovableObject::inside(MovableObject &other, bool xAxis) {
        if (xAxis) return this->x <= other.x + other.width && this->x >= other.x - other.width;
        return false;
    }

void MovableObject::hit() {
    health -=0.01;
}

void MovableObject::shoot() {
    arsenal -=0.01;
    if (arsenal < 0) {
        arsenal = 0;
    }
}

bool MovableObject::canShoot() {
    return arsenal > 0;
}
