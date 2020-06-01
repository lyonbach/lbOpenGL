#pragma once

#include "GL/glew.h"
#include <vector>

// Textures
#define BMP "bmp"

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
        std::string getFileExtension(const char* filePath) const;
        GLuint* loadBMP(const char* filePath) const;
    public:
        Helpers(const char* filePath);
        void getConfiguration(Configuration* config);
        GLuint* loadTexture(const char* filePath) const;
};
