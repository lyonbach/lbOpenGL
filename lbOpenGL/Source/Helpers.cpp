#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>

#include "Helpers.h"


Helpers::Helpers(const char* configurationsFilePath)
{

    std::ifstream stream(configurationsFilePath);

    if(stream.good())
    {
        std::cout << "[DEBUG]: Reading configurations file:\n" << configurationsFilePath << std::endl;
        std::string line;

        while(std::getline(stream, line))
        {
            if(line.find("[Width]:") != std::string::npos) m_Configuration.m_windowWidth = std::stoi(line.substr(8));
            if(line.find("[Height]:") != std::string::npos) m_Configuration.m_windowHeight = std::stoi(line.substr(9));
            if(line.find("[Title]:") != std::string::npos) m_Configuration.m_windowTitle = line.substr(8);
            if(line.find("[ShadersPath]:") != std::string::npos) m_Configuration.m_shadersFilePath = line.substr(14);
        }
    } else 
    {
        std::cout << "[ERROR]: Reading configurations file failed!:\n\t\"" << configurationsFilePath << '\"' << std::endl;
    }
}