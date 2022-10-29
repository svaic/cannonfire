#include <utility>
#include <valarray>
#include "Shape.hpp"
#include <glm/gtc/type_ptr.hpp>

Shape::Shape(std::vector<float> vertices): VaoModel(vertices) {
    this->vertices = std::move(vertices);
}

Shape Shape::createCircle(float radius, glm::vec3 color) {
    std::vector<float> vertices;
    float const PI_OVER_4 = glm::quarter_pi<float>()/4;

    float angle = 0;

    for (auto i=0; i<3; ++i)
        vertices.push_back(0.0f);

    vertices.push_back(color.x);
    vertices.push_back(color.y);
    vertices.push_back(color.z);

    for (auto i=0; i<33; ++i) {
        vertices.push_back(glm::cos(angle)*radius);
        vertices.push_back(glm::sin(angle)*radius);
        vertices.push_back(0.0f);
        vertices.push_back(color.x);
        vertices.push_back(color.y);
        vertices.push_back(color.z);
        angle += PI_OVER_4;
    }

    return {vertices};
}

Shape Shape::createRectangle() {
    return Shape(std::vector<float>{1.0f});
}
