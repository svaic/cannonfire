#include "Obstacle.hpp"
#include "Rectangle.hpp"
#include "Color.hpp"
#include "DefaultShape.hpp"

Obstacle::Obstacle()
  : MovableObject(RandomUtility::getRandomFloat(-2.5,-1.5),
                  RandomUtility::getRandomFloat(-0.5,0.7),
                  RandomUtility::getRandomFloat(0.005, 0.01),
                  RandomUtility::getRandomFloat(0.2,0.2),
                  RandomUtility::getRandomFloat(0.2,0.2)) {

    canBeDestroyed = true;//RandomUtility::getRandomBool();
    health = RandomUtility::getRandomFloat(0.5, 1.0);

    //RectangleShape shape = RectangleShape::createRectangle(1.00, 1.00, Color::BLACK);
    if (canBeDestroyed) {
        add(DefaultShape::blackObstacle);
    } else {
        add(DefaultShape::blackObstacle);
    }
}

void Obstacle::changeColor(glm::vec3 color) {
    shapes.clear();
    if (color == Color::BLACK) {
        add(DefaultShape::blackObstacle);
    }
    if (color == Color::GREEN) {
        add(DefaultShape::greenObstacle);
    }
    if (color == Color::RED) {
        add(DefaultShape::redObstacle);
    }
}

void Obstacle::move() {
    MovableObject::moveRandomX(false);
}

bool Obstacle::reduceHealth() {
    if(canBeDestroyed) {
        health -= 0.15;

        if(health <= 0.7 && health > 0.4) {
            changeColor(Color::RED);
        } else if (health <= 0.4) {
            changeColor(Color::GREEN);
        }

        //height = health;

        if (health <= 0) return true;
        else return false;
    }

    return false;
}
