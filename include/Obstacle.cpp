#include "Obstacle.hpp"
#include "Rectangle.hpp"
#include "Color.hpp"
#include "DefaultShape.hpp"

Obstacle::Obstacle()
  : MovableObject(RandomUtility::getRandomFloat(-2.5,-1.5),
                  RandomUtility::getRandomFloat(-0.3,0.5),
                  RandomUtility::getRandomFloat(0.005, 0.01)) {

    canBeDestroyed = true;//RandomUtility::getRandomBool();
    health = RandomUtility::getRandomFloat(0.5, 1.0);

    //RectangleShape shape = RectangleShape::createRectangle(1.00, 1.00, Color::BLACK);

    int randomPlanet = RandomUtility::getRandomInt(0,3);
    if (randomPlanet == 0) {
        add(DefaultShape::blackObstacle);
        width = 0.10;
        height = 0.15;
    } else if (randomPlanet == 1){
        add(DefaultShape::greenObstacle);
        width = 0.05;
        height = 0.10;
    } else if (randomPlanet == 2) {
        add(DefaultShape::redObstacle);
        width = 0.15;
        height = 0.18;
    } else {
        add(DefaultShape::asteroidField);
        width = 0.18;
        height = 0.25;
    }
}

void Obstacle::move() {
    MovableObject::moveRandomX(false);
}

bool Obstacle::reduceHealth() {
    if(canBeDestroyed) {
        health -= 0.15;

        if(health <= 0.7 && health > 0.4) {
            //changeColor(Color::RED);
        } else if (health <= 0.4) {
            //changeColor(Color::GREEN);
        }

        //height = health;

        if (health <= 0) return true;
        else return false;
    }

    return false;
}
