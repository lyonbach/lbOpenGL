#pragma once

#include <string>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>

struct Configuration
{
    int m_windowWidth;
    int m_windowHeight;
    std::string m_windowTitle;
    std::string m_shadersFilePath;
};

class Helpers
{
    public:
        Helpers(const char* configurationsFilePath);
    public:
        Configuration m_Configuration;

};