#ifndef VAO_HPP
#define VAO_HPP

#include "glad/glad.h"

class VaoModel {
public:
    GLuint *VAO;
    GLuint *VBO;

    VaoModel(std::vector<float> &vertices, bool useTexture);

    virtual void draw();

    void destructor();
};


#endif //VAO_HPP
