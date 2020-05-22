#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

struct configurations
{
    configurations(){}
    int width;
    int height;
    std::string title;
    std::string shadersFilePath;

};

static const struct
{
    float x, y;
    float r, g, b;
}
vertices[3] = {
{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
{  0.6f, -0.4f, 0.f, 1.f, 0.f },
{   0.f,  0.6f, 0.f, 0.f, 1.f } };

//// Helper Functions //
static const configurations configure(const char* filePath)
{
    std::cout << "[DEBUG]: Reading configurations file:\n" << filePath << std::endl;

    std::ifstream stream(filePath);
    std::string line;
    configurations config;

    while(std::getline(stream, line))
    {
        if(line.find("[Width]:") != std::string::npos) config.width = std::stoi(line.substr(8));
        if(line.find("[Height]:") != std::string::npos) config.height = std::stoi(line.substr(9));
        if(line.find("[Title]:") != std::string::npos) config.title = line.substr(8);
        if(line.find("[ShadersPath]:") != std::string::npos) config.shadersFilePath = line.substr(14);
    }

    return config;
}


static void cb_KeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


static void cb_FrameBuffsize(GLFWwindow* window, int width, int height)
{
    std::cout << "[DEBUG]: Inside: the cb_FrameBuffsize function..." << std::endl;
    std::cout << "[DEBUG]: Width:" << width << std::endl;
    std::cout << "[DEBUG]: Height:" << height << std::endl;
    glViewport(0, 0, width, height);
}

//// Shaders Helper Functions //
const void readShadersFile(const char* filePath, std::string* vertexShaderString, std::string* fragmentShaderString)
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


GLuint createOneShader(GLuint shaderType, const char* shaderString)
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


GLuint createShaders(const char* shadersFilePath)
{
    std::cout << "[INFO]: Creating shaders..." << std::endl;

    std::string vertexShaderString;
    std::string fragmentShaderString;
    readShadersFile(shadersFilePath, &vertexShaderString, &fragmentShaderString);

    GLchar errorLog;
    GLint compilationSuccess;
    GLuint shaderProgram = glCreateProgram();
    GLuint vertexShader = createOneShader(GL_VERTEX_SHADER, vertexShaderString.c_str());
    GLuint fragmentShader = createOneShader(GL_FRAGMENT_SHADER, fragmentShaderString.c_str());

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &compilationSuccess);
    if (compilationSuccess == 0) {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, &errorLog);
        std::cerr << "[ERROR]: Could not link shader program!\n" << 
        errorLog << std::endl;
        return 0;
    }
    return shaderProgram;
}
// Shaders Helper Functions ////
// Helper Functions ////



int main(int argc, char const *argv[])
{

    std::string shadersPath;
    const char* windowName = "Window";
    int windowWidth = 640;
    int windowHeight = 480;

    if(argc > 1)
    {
        configurations config = configure(argv[1]);
        windowWidth = config.width;
        windowHeight = config.height;
        windowName = config.title.c_str();
        shadersPath = config.shadersFilePath;
    }

    std::cout << "[INFO]: \"" << windowName << "\"" << std::endl;

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "[ERROR]: Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // Create Window
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    // Configure
    glfwSwapInterval(1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Make the window's context current.
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, cb_KeyPress);
    glfwSetFramebufferSizeCallback(window, cb_FrameBuffsize);

    // GLEW
    if (glewInit())
    {
        std::cout << "[ERROR]: Unable to initialize glew!" << std::endl;
        return -1;
    }

    // Generate Buffers
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create Shaders
    GLuint program = createShaders(shadersPath.c_str());

    GLint vpos_location, vcol_location;
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 2));

    // Main Loop
    while(!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT);

        // Rendering is done here.
        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events.
        glfwPollEvents();
    }

    return 0;
}
