#ifndef OPENGLPRJ_SHAPE_HPP
#define OPENGLPRJ_SHAPE_HPP

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "VaoModel.hpp"

class Shape : public VaoModel {
public:
    glm::vec3 position;
    Shape(std::vector<float> vertices, glm::vec3 initialPosition = glm::vec3(0.0));

    std::vector<float> vertices;

    static std::vector<float> mapToColor(std::vector<float> vertices, glm::vec3 color);

    void transform(int shaderId, glm::vec2 move, glm::vec2 scale, float angle);
};

#endif //OPENGLPRJ_SHAPE_HPP
