#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "ModelManager.h"

const char* modelPath1  = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Models/Cube.obj";
const char* modelPath2  = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Models/Teapot.obj";
const char* modelPath3  = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Models/Teapot.obj";
const char* modelPath4  = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Models/Suzanne.obj";
const char* modelPath5  = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Models/Suzanne.uns";

int main(int argc, char const *argv[])
{
    ModelManager modelManager;
    modelManager.Push(modelPath1);
    // ModelManager.Push(modelPath2);
    // ModelManager.Push(modelPath3);
    // ModelManager.Push(modelPath4);
    // ModelManager.Push(modelPath5);
    modelManager.Load();

    std::vector<const char*> modelFiles = modelManager.GetFilesList();
    std::vector<ModelData*> modelDatas = modelManager.GetModelDataArray();

    std::cout << modelFiles.at(0) << std::endl;
    ModelData md;
    md = *modelDatas.at(0);

    for (auto e : md.indexBuffer)
        std::cout << e << std::endl;

    std::cout << "################################################################################" << std::endl;

    for (auto e : md.vertexCoordsBuffer)
        std::cout << e << std::endl;

    std::cout << "################################################################################" << std::endl;

    for (auto e : md.textureCoordsBuffer)
        std::cout << e << std::endl;

    std::cout << "################################################################################" << std::endl;

    for (auto e : md.vertexNormalsBuffer)
        std::cout << e << std::endl;


    return 0;
}
