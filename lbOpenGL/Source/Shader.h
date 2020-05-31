#pragma once

#include <cstring>
#include <fstream>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Shader
{
private:
    GLuint m_Id;
    void readShadersFile(const char* filePath, std::string* vertexShaderString, std::string* fragmentShaderString);
    GLuint createOneShader(GLuint shaderType, const char* shaderString);
public:
    Shader(const char* filePath);
    ~Shader();
    void createShaderProgram(const char* filePath);
    GLuint getShaderProgram();
    void On () const;
    void Off () const;
};
