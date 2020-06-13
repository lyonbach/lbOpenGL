#pragma once

#include "GL/glew.h"
#include <vector>
#include "Containers.h"
// Textures
#define BMP "bmp"

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
