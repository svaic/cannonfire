#include "Circle.hpp"

#include <utility>
#include "Shape.hpp"
#include "GLFW/glfw3.h"

Circle::Circle(std::vector<float> vertices) : Shape(std::move(vertices), "") {
}

void Circle::draw() {
    VaoModel::draw();
    glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertices.size()/6);
}

Circle Circle::createCircle(float radius, glm::vec3 color) {
    std::vector<float> vertices;
    float const PI_OVER_4 = glm::quarter_pi<float>()/4;

    float angle = 0;

    for (auto i=0; i<2; ++i)
        vertices.push_back(0.0f);

    for (auto i=0; i<33; ++i) {
        vertices.push_back(glm::cos(angle)*radius);
        vertices.push_back(glm::sin(angle)*radius);
        angle += PI_OVER_4;
    }

    return {mapToColor(vertices, color)};
}