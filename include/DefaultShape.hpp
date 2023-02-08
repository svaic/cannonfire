//
// Created by ljovanov on 11.12.2022.
//

#ifndef OPENGLPRJ_DEFAULTSHAPE_HPP
#define OPENGLPRJ_DEFAULTSHAPE_HPP


#include "Shape.hpp"
#include "Rectangle.hpp"

class DefaultShape {
public:
    static RectangleShape* blackObstacle;
    static RectangleShape* greenObstacle;
    static RectangleShape* redObstacle;
    static RectangleShape* asteroidField;
    static void setYellowObstacle(RectangleShape& newBlackObstacle);
    static void setGreenObstacle(RectangleShape& newGreenObstacle);
    static void setRedObstacle(RectangleShape& newRedObstacle);
    static void setAsteroidField(RectangleShape& newAsteroidField);
};


#endif //OPENGLPRJ_DEFAULTSHAPE_HPP
