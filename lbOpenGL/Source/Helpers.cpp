#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>

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
        std::cout << "[DEBUG][HELPERS]: Reading configurations file:\n" << m_configurationsFilePath << std::endl;
        std::string line;

        while(std::getline(stream, line))
        {
            if(line.find("[Width]:") != std::string::npos) config->m_windowWidth = std::stoi(line.substr(8));
            if(line.find("[Height]:") != std::string::npos) config->m_windowHeight = std::stoi(line.substr(9));
            if(line.find("[Title]:") != std::string::npos) config->m_windowTitle = line.substr(8);
            if(line.find("[ShadersPath]:") != std::string::npos) config->m_shadersFilePath = line.substr(14);
        }
        config->ok = true;
    } else 
    {
        std::cout << "[ERROR][HELPERS]: Reading configurations file failed!:\n\t\"" << m_configurationsFilePath << '\"' << std::endl;
        config->ok = false;
    }
}