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

void MovableObject::moveRandomX(bool changeSideRandom) {
    if (changeSideRandom) changeSide();
    if (x - speed <= -1.00 && left == true) {
        left = false;
    }
    else if (x + speed >= 1.00 && left == false) {
        left = true;
    }
    if (left) {
        moveX(-speed);
    } else {
        moveX(speed);
    }
}

void MovableObject::moveX(float offset) {
    x += offset;

    draw();
}

void MovableObject::moveY(float offset) {
    if (offset == -1) {
        y += speed;
    } else {
        y += offset;
    }

    draw();
}

void MovableObject::changeSide() {
    if (iteration == 10) {
        iteration = 0;

        speed = RandomUtility::getRandomFloat(0.05, 0.01);

        int shouldChangeDirectionCoinFlip = RandomUtility::getRandomInt(0,10);
        if (shouldChangeDirectionCoinFlip > 7)
            left = !left;
    }
    iteration++;
}

void MovableObject::reduceHealth() {
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

bool MovableObject::collide(MovableObject &other)  {
    return this->x <= other.x + other.width && this->x >= other.x - other.width && std::abs(this->y - other.y) <= 0.05;
}

void MovableObject::draw() {
    transform(glm::vec2(x, y), glm::vec2(width, height), 0);
}

void MovableObject::changeWidth(float newWidth) {
    this->width = newWidth;
    draw();
}

void MovableObject::changeSpeed(float newSpeed) {
    this->speed = newSpeed;
}
