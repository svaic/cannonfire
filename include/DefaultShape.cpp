#include "DefaultShape.hpp"

// void DefaultShape::setGreenObstacle(RectangleShape& newGreenObstacle)
void DefaultShape::setGreenObstacle(RectangleShape& newGreenObstacle)  {
    greenObstacle = &newGreenObstacle;
}

void DefaultShape::setBlackObstacle(RectangleShape &newBlackObstacle) {
    blackObstacle = &newBlackObstacle;
}

void DefaultShape::setRedObstacle(RectangleShape &newRedObstacle) {
    redObstacle = &newRedObstacle;
}
