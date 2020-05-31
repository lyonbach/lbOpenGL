#include "Texture.h"

Texture::Texture(unsigned int width, unsigned int height, GLuint* textureData)
{
    glGenTextures(1, &m_Id);
    glBindTexture(GL_TEXTURE_2D, m_Id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(m_Id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_Id);
}

void Texture::On() const
{
    glBindTexture(GL_TEXTURE_2D, m_Id);
}

void Texture::Off() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}