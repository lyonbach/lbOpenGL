#pragma once

#include <vector>

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