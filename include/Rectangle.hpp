#ifndef OPENGLPRJ_RECTANGLE_HPP
#define OPENGLPRJ_RECTANGLE_HPP


#include "Shape.hpp"

class RectangleShape : public Shape {
public:
    GLuint *EBO;
    RectangleShape(std::vector<float> vertices, std::vector<int> indices = std::vector<int>());
    void draw();
    static RectangleShape createRectangle(std::vector<float> vertices, glm::vec3 color);
    static RectangleShape createRectangle(float width, float height, glm::vec3 color);
    static RectangleShape createTriangle(std::vector<float> vertices, glm::vec3 color, glm::vec2 initialPos);
    void destructor();
};


#endif //OPENGLPRJ_RECTANGLE_HPP
