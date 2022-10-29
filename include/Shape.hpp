#ifndef OPENGLPRJ_SHAPE_HPP
#define OPENGLPRJ_SHAPE_HPP

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "VaoModel.hpp"

class Shape : public VaoModel {
public:
    Shape(std::vector<float> vertices);

    std::vector<float> vertices;

    static Shape createCircle(float radius, glm::vec3 color);

    static Shape createRectangle();
};

#endif //OPENGLPRJ_SHAPE_HPP
