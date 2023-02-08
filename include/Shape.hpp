#ifndef OPENGLPRJ_SHAPE_HPP
#define OPENGLPRJ_SHAPE_HPP

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "VaoModel.hpp"
#include <string>
#include <iostream>

class Shape : public VaoModel {
public:
    static unsigned int shaderId;
    unsigned int texture;

    Shape(std::vector<float> vertices, bool useTexture);

    std::vector<float> vertices;

    static std::vector<float> mapToColor(std::vector<float> vertices, glm::vec3 color);

    static std::vector<float> mapToTexture(std::vector<float> vertices);

    static void setShaderId(unsigned int id);

    void loadTexture(std::string name);

    void transform(glm::vec2 move, glm::vec2 scale, float angle);

    void move(glm::vec2 move);

    void scale(glm::vec2 scale);

    void rotate(float angle);
};

#endif //OPENGLPRJ_SHAPE_HPP
