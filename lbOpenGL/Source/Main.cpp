#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GLM/matrix.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "Helpers.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArrayObject.h"

const char* configPath = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Config/Main.cfg";
const char* texturePath = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Textures/test_pattern.bmp";
const char* shadersPath = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Shaders/Shaders.shd";
const char* shadersPath2 = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Shaders/TextureShader.shd";

const float vertices[6 * 3] = {
    -0.6f, -0.4f, 0.0f, /*position*/ 1.0f, 0.0f, 0.0f, /*color*/
     0.6f, -0.4f, 0.0f, /*position*/ 0.0f, 1.0f, 0.0f, /*color*/
     0.0f,  0.6f, 0.0f, /*position*/ 0.0f, 0.0f, 1.0f  /*color*/
};

const unsigned int indices[3] = {
    0, 1, 2  // Element numbers.
};

const float vertices2[6 * 4] = {
    -0.8f, -0.4f, 0.0f, /*position*/ 0.0f, 0.0f, 0.0f, /*UVs*/
    -0.8f,  0.6f, 0.0f, /*position*/ 0.0f, 1.0f, 0.0f, /*UVs*/
     0.8f, -0.4f, 0.0f, /*position*/ 1.0f, 0.0f, 0.0f, /*UVs*/
     0.8f,  0.6f, 0.0f, /*position*/ 1.0f, 1.0f, 0.0f, /*UVs*/
};

const unsigned int indices2[3 * 2] = {
    0, 2, 3,  // Element numbers.
    0, 3, 1  
};

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

int main(int argc, char const *argv[])
{

    // Get Configurations
    Helpers helpers(configPath);
    Configuration config;
    helpers.getConfiguration(&config);
    if (!config.ok)
    {
        std::cout << "[ERROR][MAIN]: Configurations could not be set properly. Aborted." << std::endl;
        return -1;
    }
    else
    {
        std::cout << "[DEBUG][MAIN]: Configurations were successfully set, starting..." << std::endl;
    }

    const int windowWidth = config.m_windowWidth;
    const int windowHeight = config.m_windowHeight;
    const char* windowName = config.m_windowTitle.c_str();

    std::cout << "[INFO][MAIN]: \"" << windowName << "\"" << std::endl;

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "[ERROR][MAIN]: Failed to initialize GLFW." << std::endl;
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
        std::cout << "[ERROR][MAIN]: Unable to initialize glew!" << std::endl;
        return -1;
    }



    // Generate Buffers
    VertexArrayObject vao(vertices, 3 * 6 * sizeof(float), indices, 3);
    VertexArrayObject vao2(vertices2, 4 * 6 * sizeof(float), indices2, 3 * 2);


    // Generate Textures
    Texture texture(400, 400, helpers.loadTexture(texturePath));

    // Create Shaders
    Shader shader(shadersPath);
    Shader textureShader(shadersPath2);
    GLuint program = shader.getShaderProgram();

    GLint mvp_location;
    mvp_location  = glGetUniformLocation(program, "uMVP");

    float rotationAngle = 0.0f;
    glm::mat4 model;
    glm::mat4 view = glm::mat4 (1.0f);
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glm::mat4 mvp;

    // Main Loop
    while(!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT);

        // Rendering is done here.

        // Draw call 1.
        {
            model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            glm::mat4 rotateZ = glm::rotate(rotationAngle + 5.0f, glm::vec3(0.0f, 0.0f, 1.0f));

            vao2.On();
            texture.On();
            textureShader.On();
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            textureShader.Off();
            vao2.Off();
        }

        // Draw call 2.
        {
            rotationAngle += .01f;
            model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));
            glm::mat4 rotateZ = glm::rotate(rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 translate = glm::translate(glm::vec3(0.8f, -0.8f, 0.0f));
            mvp = model * translate * rotateZ * view * projection;

            vao.On();
            shader.On();
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            shader.Off();
            vao.Off();
        }


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events.
        glfwPollEvents();
    }

    return 0;
}
