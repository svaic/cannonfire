#include <utility>
#include <valarray>
#include <iostream>
#include "Shape.hpp"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Shape::Shape(std::vector<float> vertices, bool useTexture): VaoModel(vertices, useTexture) {
    this->vertices = std::move(vertices);
}

std::vector<float> Shape::mapToColor(std::vector<float> vertices, glm::vec3 color) {
    std::vector<float> newVertices;

    for (int i = 0; i < vertices.size(); i+=2) {
        newVertices.insert(newVertices.end(), {vertices[i], vertices[i+1], 0.0, color.x, color.y, color.z});
    }

    return newVertices;
}

std::vector<float> Shape::mapToTexture(std::vector<float> vertices) {
    std::vector<float> newVertices;

    std::vector<float> textureCords = {
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
    };

    int j =0;
    for (int i = 0; i < vertices.size(); i+=6) {
        newVertices.insert(newVertices.end(), {vertices[i], vertices[i+1], vertices[i+2], vertices[i+3],vertices[i+4], vertices[i+5], textureCords[j++], textureCords[j++]});
    }

    return newVertices;
}

void Shape::transform(glm::vec2 move, glm::vec2 scale, float angle) {
    unsigned int transformLoc = glGetUniformLocation(shaderId, "transform");
    // second transformation
    // ---------------------
    glm::mat4 transform = glm::mat4(1.0f); // reset it to identity matrix
    transform = glm::translate(transform, glm::vec3(move, 0.0));
    transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(scale, 1.0));
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

void Shape::loadTexture(std::string name) {
        std::string fullname = "../res/textures/" + name;

        // load and create a texture
        // -------------------------
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char *data = stbi_load(fullname.data(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
}


//void Shape::loadTexture(std::string name)
