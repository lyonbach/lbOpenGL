#include <iostream>

#include "ModelManager.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "Helpers.h"

#define display(x) for(auto e: x) std::cout << e <<std::endl

int main(int argc, char const *argv[])
{
    std::cout << " Testing: testIndexedBufferGenerator" << std::endl;

    // Configure
    Helpers helpers("/home/lyonbach/Repositories/lbOpenGL/Config/Main.cfg");
    Configuration config;
    helpers.getConfiguration(&config);

    // Prepare model data
    ModelManager modelManager;
    modelManager.Push(config.m_modelPath.c_str());
    modelManager.Load();
    modelManager.~ModelManager();

    // Prepare texture data
    Texture texture(config.m_textureSize[0], config.m_textureSize[1], helpers.loadTexture(config.m_texturePath.c_str()));

    // Prepare shader data
    // Shader shader(config.m_shadersFilePath.c_str());
    
    // Renderer renderer;
    // Renderer renderer;
    // Model model;
    // VertexArrayObject vao;
    // vao.Init(*modelManager.GetModelDataArray().at(0));
    // renderer.Push({&vao, &shader, &texture});

    // std::cout << "Current Item Count in Renderer" << renderer.GetItemCount() << std::endl;
    // display(modelData.vertexCoordsBuffer);

    return 0;
}
