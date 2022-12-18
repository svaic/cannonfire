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
    static void setBlackObstacle(RectangleShape& newBlackObstacle);
    static void setGreenObstacle(RectangleShape& newGreenObstacle);
    static void setRedObstacle(RectangleShape& newRedObstacle);
};


#endif //OPENGLPRJ_DEFAULTSHAPE_HPP
