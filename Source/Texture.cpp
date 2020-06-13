#include <iostream>

#include "Texture.h"
#include "Definitions.h"

Texture::Texture()
{

}

void Texture::GenerateTexture(unsigned int width, unsigned int height, GLuint* textureData)
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

const char* Texture::GetTexturePath() const
{
    return m_TexturePath;
}

void Texture::SetTexturePath(const char* texturePath)
{
    m_TexturePath = texturePath;
}

void Texture::LoadTexture()
{
    if(m_TexturePath == "")
    {
        std::cout << "[ERROR][TEXTURE]: Texture path not set." << m_TexturePath << std::endl;
        std::cout << "texture.SetTexturePath(<filepath>)" << std::endl;
    }

    std::string texturePath(m_TexturePath);
    std::cout << "[DEBUG][TEXTURE]: Loading Texture from:\n\t" << texturePath << std::endl;
    std::string ext = texturePath.substr(texturePath.find_last_of('.') + 1);

    if (ext == BMP)
    {
        std::cout << "[DEBUG][TEXTURE]: Texture file-type: " << ext << std::endl;
        LoadBMP(m_TexturePath);
    }
    else
    {
        std::cout << "[ERROR][TEXTURE]: Unable to load:\n\t" << m_TexturePath << "\nCurrently only bmp files are supported." << std::endl;
        return;
    }

    std::cout << "[DEBUG][TEXTURE]: Texture load successful." << std::endl;
}


void Texture::On() const
{
    glBindTexture(GL_TEXTURE_2D, m_Id);
}

void Texture::Off() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::LoadBMP(const char* texturePath)
{
    unsigned int width, height = 0;
    unsigned char header[54];   // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;       // Position in the file where the actual data begins
    unsigned int imageSize;     // = width*height*3
    unsigned char * data;       // Actual RGB data

    FILE* file = fopen(texturePath, "rb");
    if (!file)
    {
        std::cout << "[ERROR][TEXTURE]: Unable to open texture for reading." << std::endl;
        return;
    }

    // Check if the file is really a bmp file.
    if (fread(header, 1, 54, file)!=54)
    {
        std::cout << "[ERROR][TEXTURE]: The file \n\t\"" << texturePath << "\"\nis not a proper bmp file." << std::endl;
        return;
    }

    // First two characters of the bmp file are "B" and "M" correspondingly.
    if (header[0]!='B' || header[1]!='M'){
        std::cout << "[ERROR][TEXTURE]: The file \n\t\"" << texturePath << "\"\nis not a proper bmp file." << std::endl;
        return;
    }

    // Now read the file.
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);


    if(width == 0)
        std::cout << "[WARNING][TEXTURE]: Could not determine width, there might be some unexpexted results."  << std::endl; 
    if(height == 0)
        std::cout << "[WARNING][TEXTURE]: Could not determine height, there might be some unexpexted results."  << std::endl; 

    // Some BMP files are misformatted, if so, missing information should be fetched.
    // 3 : one byte for each Red, Green and Blue component.
    if (imageSize==0) imageSize=width*height*3;
    // The proper BMP header.
    if (dataPos==0) dataPos=54;

    // Allocate.
    data = new unsigned char [imageSize];
    // Read the actual data from the file into the allocated buffer.
    fread(data, 1, imageSize, file);

    // Close the file.
    fclose(file);

    GenerateTexture(width, height, (GLuint*)data);

    // Cleanup
    delete data;
}
