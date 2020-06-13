#pragma once

#include "GL/glew.h"

class Texture
{
private:
    unsigned int m_Id;
    const char* m_TexturePath = "";

public:
    Texture();
    ~Texture();

    const char* GetTexturePath() const;
    void SetTexturePath(const char* texturePath);
    
    void LoadTexture();
    void GenerateTexture(unsigned int width, unsigned int height, GLuint* textureData);
    void On() const;
    void Off() const;

private:
    void LoadBMP(const char* texturePath);

};