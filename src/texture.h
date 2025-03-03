#ifndef TEXTURE_H
#define TEXTURE_H

#define TEXTURE_DIFFUSE 0
#define TEXTURE_SPECULAR 1

#include <string>
#include <glad/glad.h>

class Texture
{
public:
    unsigned int ID;
    int width;
    int height;
    int nrChannels;
    std::string path;

    int textureType;

    Texture(const GLchar* texturePath);

    void use(GLenum texture);
};

#endif