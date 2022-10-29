#ifndef VAO_HPP
#define VAO_HPP

#include "glad/glad.h"

class VaoModel {
public:
    GLuint *VAO;
    GLuint *VBO;

    VaoModel(std::vector<float> &vertices);

    virtual void draw();

    void desctruct();
};


#endif //VAO_HPP
