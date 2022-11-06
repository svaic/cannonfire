#include "ShapeContainer.hpp"


void ShapeContainer::add(Shape* shape) {
    shapes.push_back(shape);
}

void ShapeContainer::draw() {
    for (auto & shape : shapes) {
        shape->draw();
    }
}

void ShapeContainer::transform(glm::vec2 move, glm::vec2 scale, float angle) {
    unsigned int transformLoc = glGetUniformLocation(shaderId, "transform");
    // second transformation
    // ---------------------
    glm::mat4 transform = glm::mat4(1.0f); // reset it to identity matrix
    transform = glm::translate(transform, glm::vec3(move, 0.0));
    transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(scale, 1.0));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    draw();
}

void ShapeContainer::move(glm::vec2 move) {
    transform(move, glm::vec2(1.0), 0);
}

void ShapeContainer::scale(glm::vec2 scale) {
    transform(glm::vec2(0.0), scale, 0);
}

void ShapeContainer::rotate(float angle) {
    transform(glm::vec2(0.0), glm::vec2(1.0), angle);
}

void ShapeContainer::setShaderId(unsigned int id) {
    shaderId = id;
}
