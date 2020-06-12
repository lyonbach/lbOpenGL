#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "Helpers.h"


Helpers::Helpers(const char* filePath)
    : m_configurationsFilePath(filePath)
{
}

void Helpers::getConfiguration(Configuration* config)
{

    std::ifstream stream(m_configurationsFilePath);

    if(stream.good())
    {
        std::cout << "[DEBUG][HELPERS]: Reading configurations file from:\n\t" << m_configurationsFilePath << std::endl;
        std::string line;

        while(std::getline(stream, line))
        {
            if(line.find("[Width]:") != std::string::npos) config->m_windowWidth = std::stoi(line.substr(8));
            if(line.find("[Height]:") != std::string::npos) config->m_windowHeight = std::stoi(line.substr(9));
            if(line.find("[Title]:") != std::string::npos) config->m_windowTitle = line.substr(8);
            if(line.find("[TexturePath]:") != std::string::npos) config->m_texturePath = line.substr(14);
            if(line.find("[TextureSize]:") != std::string::npos)
            {
                std::stringstream resolutionStream(line.substr(14));
                std::string size;
                int c = 0;
                while(std::getline(resolutionStream, size, 'X'))
                {
                    config->m_textureSize[c] = std::stoi(size);
                    c++;
                }
            }
            if(line.find("[ShadersPath]:") != std::string::npos) config->m_shadersFilePath = line.substr(14);
            if(line.find("[ModelPath]:") != std::string::npos) config->m_modelPath = line.substr(12);

        }
        config->ok = true;
    } else
    {
        std::cout << "[ERROR][HELPERS]: Reading configurations file failed!:\n\t\"" << m_configurationsFilePath << '\"' << std::endl;
        config->ok = false;
    }
}

std::string Helpers::getFileExtension(const char* filePath) const
{
    std::string tp = std::string(filePath);
    return tp.substr(tp.find_last_of('.') + 1);
}

GLuint* Helpers::loadTexture(const char* filePath) const
{
    std::cout << "[DEBUG][HELPERS]: Loading Texture from: \n\t" << filePath << std::endl;
    std::string ext = getFileExtension(filePath);
    GLuint* loaded;
    if (ext == BMP)
    {
        std::cout << "[DEBUG][HELPERS]: Texture file-type: " << ext << std::endl;
        loaded = loadBMP(filePath);
    }
    else
    {
        std::cout << "[ERROR][HELPERS]: Unable to load:\n\t" << filePath << "\nCurrently only bmp files are supported." << std::endl;
        return nullptr;
    }

    std::cout << "[DEBUG][HELPERS]: Texture load successful." << std::endl;
    return loaded;
}

GLuint* Helpers::loadBMP(const char* filePath) const
{
    unsigned int width, height;
    unsigned char header[54];   // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;       // Position in the file where the actual data begins
    unsigned int imageSize;     // = width*height*3
    unsigned char * data;       // Actual RGB data

    FILE* file = fopen(filePath, "rb");
    if (!file)
    {
        std::cout << "[ERROR][HELPERS]: Unable to open texture for reading." << std::endl;
        return nullptr;
    }

    // Check if the file is really a bmp file.
    if (fread(header, 1, 54, file)!=54)
    {
        std::cout << "[ERROR][HELPERS]: The file \n\t\"" << filePath << "\"\nis not a proper bmp file." << std::endl;
        return nullptr;
    }

    // First two characters of the bmp file are "B" and "M" correspondingly.
    if (header[0]!='B' || header[1]!='M'){
        std::cout << "[ERROR][HELPERS]: The file \n\t\"" << filePath << "\"\nis not a proper bmp file." << std::endl;
        return nullptr;
    }

    // Now read the file.
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, if so, missing information should be fetched.
    // 3 : one byte for each Red, Green and Blue component.
    if (imageSize==0)    imageSize=width*height*3;
    // The proper BMP header.
    if (dataPos==0)      dataPos=54;

    // Allocate.
    data = new unsigned char [imageSize];
    // Read the actual data from the file into the allocated buffer.
    fread(data, 1, imageSize, file);
    // Close the file.
    fclose(file);

    return (GLuint*)data;
}
