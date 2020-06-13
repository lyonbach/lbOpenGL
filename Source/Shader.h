#pragma once

#include <cstring>
#include <fstream>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Shader
{
private:
    unsigned int m_Id;
    const char* m_ShadersFilePath;
    void ReadShadersFile(const char* filePath, std::string* vertexShaderString, std::string* fragmentShaderString);
    GLuint CreateOneShader(GLuint shaderType, const char* shaderString);
public:
    Shader();
    ~Shader();

    const char* GetShadersFilePath() const;
    GLuint GetShaderProgram() const;

    void SetShaderFilePath(const char* shaderFilePath);

    void GenerateShaders();
    void On () const;
    void Off () const;
};
