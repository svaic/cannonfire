#ifndef OPENGLPRJ_RECTANGLE_HPP
#define OPENGLPRJ_RECTANGLE_HPP


#include "Shape.hpp"

class RectangleShape : public Shape {
public:
    GLuint *EBO;
    RectangleShape(std::vector<float> vertices, std::vector<int> indices = std::vector<int>());
    void draw();
    static RectangleShape createRectangle(std::vector<float> vertices, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f));
    static RectangleShape createTriangle(std::vector<float> vertices, glm::vec3 color);

    void desctruct();
};


#endif //OPENGLPRJ_RECTANGLE_HPP
