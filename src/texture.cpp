#include "texture.h"
#include <stb_image.h>

Texture::Texture(const GLchar* texturePath)
{
    this->path = texturePath;

    stbi_set_flip_vertically_on_load(true);
    
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    glGenTextures(1, &ID);

    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::use(GLenum texture)
{
    glActiveTexture(texture);
    glBindTexture(GL_TEXTURE_2D, ID);
}