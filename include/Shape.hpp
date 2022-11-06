#ifndef OPENGLPRJ_SHAPE_HPP
#define OPENGLPRJ_SHAPE_HPP

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "VaoModel.hpp"

class Shape : public VaoModel {
public:
    static unsigned int shaderId;

    Shape(std::vector<float> vertices);

    std::vector<float> vertices;

    static std::vector<float> mapToColor(std::vector<float> vertices, glm::vec3 color);

    static void setShaderId(unsigned int id);

    void transform(glm::vec2 move, glm::vec2 scale, float angle);

    void move(glm::vec2 move);

    void scale(glm::vec2 scale);

    void rotate(float angle);
};

#endif //OPENGLPRJ_SHAPE_HPP
