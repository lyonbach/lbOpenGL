#include <iostream>

#include "Shader.h"


Shader::Shader(const char* filePath)
{
    createShaderProgram(filePath);
}

Shader::~Shader()
{
    glDeleteProgram(m_Id);
}

void Shader::readShadersFile(const char* filePath, std::string* vertexShaderString, std::string* fragmentShaderString)
{
    std::cout << "[DEBUG]: Reading shaders file:\n" << filePath << std::endl;
    std::ifstream stream(filePath);
    std::string line;
    std::string* current;

    while (std::getline(stream, line))
    {
        if (line.find("#Shader") != std::string::npos)
        {
            if (line.find("Vertex") != std::string::npos)
            {
                current = vertexShaderString;
                std::cout << "[DEBUG]: Filling vertex shader string." << std::endl;
            }
            else if (line.find("Fragment") != std::string::npos)
            {
                current = fragmentShaderString;
                std::cout << "[DEBUG]: Filling fragment shader string." << std::endl;
            }
        }
        else { current->append(line + '\n'); }
    }
}

GLuint Shader::createOneShader(GLuint shaderType, const char* shaderString)
{

    GLint compilationSuccess;

    GLuint shaderObject  = glCreateShader(shaderType);
    const GLchar* p[1];
    p[0] = shaderString;
    GLint Lengths[1];
    Lengths[0]= strlen(shaderString);
    glShaderSource(shaderObject, 1, p, Lengths);
    glCompileShader(shaderObject);

    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compilationSuccess);
    if (!compilationSuccess) {

        GLchar errorLog[1024];
        glGetShaderInfoLog(shaderObject, sizeof(errorLog), NULL, errorLog);
        std::cerr << "[ERROR]: Unable to compile: " << ((shaderType == GL_VERTEX_SHADER) ? "vertex shader" : "fragment shader")
        << '\n' << errorLog << std::endl;
        return 0;
    }

    return shaderObject;
}

void Shader::createShaderProgram(const char* filePath)
{
    glDeleteProgram(m_Id);
    m_Id = glCreateProgram();

    std::cout << "[INFO][SHADER]: Creating shaders..." << std::endl;
    std::string vertexShaderString;
    std::string fragmentShaderString;
    readShadersFile(filePath, &vertexShaderString, &fragmentShaderString);

    GLchar errorLog;
    GLint compilationSuccess;
    GLuint vertexShader = createOneShader(GL_VERTEX_SHADER, vertexShaderString.c_str());
    GLuint fragmentShader = createOneShader(GL_FRAGMENT_SHADER, fragmentShaderString.c_str());

    glAttachShader(m_Id, vertexShader);
    glAttachShader(m_Id, fragmentShader);
    glLinkProgram(m_Id);
    glValidateProgram(m_Id);

    glGetProgramiv(m_Id, GL_LINK_STATUS, &compilationSuccess);
    if (compilationSuccess == 0) {
        glGetProgramInfoLog(m_Id, sizeof(errorLog), NULL, &errorLog);
        std::cerr << "[ERROR][SHADER]: Could not link shader program!\n" << 
        errorLog << std::endl;
        return;
    }
}

GLuint Shader::getShaderProgram()
{
    return m_Id;
}

void Shader::On() const
{
    glUseProgram(m_Id);
}

void Shader::Off() const
{
    glUseProgram(0);
}