#include <utility>
#include <valarray>
#include <iostream>
#include "Shape.hpp"

Shape::Shape(std::vector<float> vertices): VaoModel(vertices) {
    this->vertices = std::move(vertices);
    this->position = glm::vec2(0.0, 0.0);
    this->test = 0;
}

std::vector<float> Shape::mapToColor(std::vector<float> vertices, glm::vec3 color) {
    std::vector<float> newVertices;

    for (int i = 0; i < vertices.size(); i+=2) {
        newVertices.insert(newVertices.end(), {vertices[i], vertices[i+1], 0.0, color.x, color.y, color.z});
    }

    return newVertices;
}

void Shape::transform(glm::vec2 move, glm::vec2 scale, float angle) {
    unsigned int transformLoc = glGetUniformLocation(shaderId, "transform");
    // second transformation
    // ---------------------
    glm::mat4 transform = glm::mat4(1.0f); // reset it to identity matrix
    transform = glm::translate(transform, glm::vec3(move.x,move.y, 0.0));
    transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(scale, 1.0));

    /*position.x += move.x;
    position.y += move.y;*/

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

void Shape::move(glm::vec2 move) {
    transform(move, glm::vec2(1.0), 0);
}

void Shape::scale(glm::vec2 scale) {
    transform(glm::vec2(0.0), scale, 0);
}

void Shape::rotate(float angle) {
    transform(glm::vec2(0.0), glm::vec2(1.0), angle);
}

void Shape::setShaderId(unsigned int id) {
    shaderId = id;
}
