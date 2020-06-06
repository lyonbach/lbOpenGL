#include <iostream>

#define display(x) for(auto e: x) std::cout << e <<std::endl

#include "OBJLoader.h"

const char* modelPath  = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Models/Cube.obj";

int main(int argc, char const *argv[])
{
    std::cout << " Testing: testIndexedBufferGenerator" << std::endl;

    OBJLoader loader(modelPath);
    ModelData modelData;
    
    loader.Load(&modelData, true);

    display(modelData.vertexCoordsBuffer);
    // display(modelData.textureCoordsBuffer);
    // display(modelData.vertexNormalsBuffer);
    // display(modelData.indexBuffer);
    // std::cout << modelData.indexBuffer.size() << std::endl;

    return 0;
}
