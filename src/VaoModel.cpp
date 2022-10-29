#include <vector>
#include "VaoModel.hpp"
#include "glad/glad.h"

VaoModel::VaoModel(std::vector<float> &vertices) {

    VAO = new GLuint;
    VBO = new GLuint;
    glGenVertexArrays(ctr, VAO);
    glGenBuffers(ctr, VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyway, so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    ctr++;
}

void VaoModel::draw() {
    glBindVertexArray(*VAO);
}

void VaoModel::desctruct() {
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);
}