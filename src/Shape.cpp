#include <utility>
#include <valarray>
#include "Shape.hpp"

Shape::Shape(std::vector<float> vertices): VaoModel(vertices) {
    this->vertices = std::move(vertices);
}

std::vector<float> Shape::mapToColor(std::vector<float> vertices, glm::vec3 color) {
    std::vector<float> newVertices;

    for (int i = 0; i < vertices.size(); i+=2) {
        newVertices.insert(newVertices.end(), {vertices[i], vertices[i+1], 0.0, color.x, color.y, color.z});
    }

    return newVertices;
}
