#include <utility>
#include <valarray>
#include "Shape.hpp"

Shape::Shape(std::vector<float> vertices, glm::vec3 initialPosition): VaoModel(vertices) {
    this->vertices = std::move(vertices);
    position = glm::vec3(initialPosition);
}

std::vector<float> Shape::mapToColor(std::vector<float> vertices, glm::vec3 color) {
    std::vector<float> newVertices;

    for (int i = 0; i < vertices.size(); i+=2) {
        newVertices.insert(newVertices.end(), {vertices[i], vertices[i+1], 0.0, color.x, color.y, color.z});
    }

    return newVertices;
}

void Shape::transform(int shaderId, glm::vec2 move, glm::vec2 scale, float angle) {
    unsigned int transformLoc = glGetUniformLocation(shaderId, "transform");
    // second transformation
    // ---------------------
    glm::mat4 transform = glm::mat4(1.0f); // reset it to identity matrix
    transform = glm::translate(transform, glm::vec3(move, 0.0));
    transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(scale, 1.0));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}
