#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:
    unsigned int ID;
    int width;
    int height;
    int nrChannels;

    Texture(const GLchar* texturePath);

    void use(GLenum texture);
};

#endif