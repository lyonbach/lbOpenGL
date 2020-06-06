#pragma once

#include "GL/glew.h"

class Texture
{
private:
    unsigned int m_Id;
public:
    Texture(unsigned int width, unsigned int height, GLuint* textureData);
    ~Texture();
    void On() const;
    void Off() const;
};
