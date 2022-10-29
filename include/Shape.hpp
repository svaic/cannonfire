#ifndef OPENGLPRJ_SHAPE_HPP
#define OPENGLPRJ_SHAPE_HPP

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "VaoModel.hpp"

class Shape : public VaoModel {
public:
    Shape(std::vector<float> vertices);

    std::vector<float> vertices;

    static std::vector<float> mapToColor(std::vector<float> vertices, glm::vec3 color);
};

#endif //OPENGLPRJ_SHAPE_HPP
