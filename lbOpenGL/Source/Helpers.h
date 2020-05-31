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
    bool ok = false;
};

class Helpers
{
    private:
        const char* m_configurationsFilePath;
    public:
        Helpers(const char* filePath);
        void getConfiguration(Configuration* config);
};