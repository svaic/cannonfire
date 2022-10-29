#ifndef VAO_HPP
#define VAO_HPP

#include "glad/glad.h"

class VaoModel {
public:
    GLuint *VAO;
    GLuint *VBO;
    static int ctr;

    VaoModel(std::vector<float> &vertices);

    void draw();

    void desctruct();
};


#endif //VAO_HPP
