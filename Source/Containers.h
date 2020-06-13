#pragma once

#include <vector>

struct Configuration
{
    int m_windowWidth;
    int m_windowHeight;
    std::string m_windowTitle;
    std::string m_shadersFilePath;
    std::string m_texturePath;
    unsigned int m_textureSize[2];
    std::string m_modelPath;
    bool ok = false;
};

struct ModelData
{
    std::vector<float> vertexBuffer;
    std::vector<float> vertexCoordsBuffer;
    std::vector<float> textureCoordsBuffer;
    std::vector<float> vertexNormalsBuffer;
    std::vector<unsigned int> indexBuffer;
};

struct UniqueBuffer
{
    unsigned int index;
    std::vector<float> data;
};