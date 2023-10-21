#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

#include "Helpers.h"

Helpers::Helpers()
{
    // m_configurationsFilePath = (std::filesystem::current_path() / "Config" / "Main.cfg").c_str();
    m_configurationsFilePath = "/media/lyonbach/work/Projects/lbOpenGL/Config/Main.cfg";
}

void Helpers::getConfiguration(Configuration* config)
{
    std::cout << "Configurations file path :\n\t" << m_configurationsFilePath << std::endl;

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
