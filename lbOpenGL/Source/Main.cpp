#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GLM/matrix.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "Helpers.h"
#include "Shader.h"
#include "VertexArrayObject.h"

const char* configPath = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Config/Main.cfg";

const float vertices[6 * 3] = {
    -0.6f, -0.4f, 0.0f, /*position*/ 1.0f, 0.0f, 0.0f, /*color*/
     0.6f, -0.4f, 0.0f, /*position*/ 0.0f, 1.0f, 0.0f, /*color*/
     0.0f,  0.6f, 0.0f, /*position*/ 0.0f, 0.0f, 1.0f  /*color*/
};

const unsigned int indices[3] = {
    0, 2, 1  // Element numbers.
};

const float vertices2[6 * 3] = {
     0.0f,  0.0f, 0.0f, /*position*/ 0.0f, 0.0f, 1.0f,  /*color*/
     0.6f, -0.4f, 0.0f, /*position*/ 0.0f, 1.0f, 0.0f, /*color*/
    -0.6f, -0.4f, 0.0f, /*position*/ 1.0f, 0.0f, 0.0f, /*color*/
};

const unsigned int indices2[3] = {
    0, 1, 2  // Element numbers.
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

    Helpers helpers(configPath);

    const int windowWidth = helpers.m_Configuration.m_windowWidth;
    const int windowHeight = helpers.m_Configuration.m_windowHeight;
    const char* windowName = helpers.m_Configuration.m_windowTitle.c_str();
    const char* shadersPath = helpers.m_Configuration.m_shadersFilePath.c_str();;

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
    VertexArrayObject vao(vertices, 3*6*sizeof(float), indices, 3);
    VertexArrayObject vao2(vertices2, 3*6*sizeof(float), indices, 3);


    // Create Shaders
    Shader shader(shadersPath);
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
        shader.On();

        // Draw call 1.
        {
            rotationAngle += .01f;
            model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            glm::mat4 rotateZ = glm::rotate(rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = model * rotateZ * view * projection;

            vao.On();
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            vao.Off();
        }

        // Draw call 2.
        {
            model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            glm::mat4 rotateZ = glm::rotate(rotationAngle + 5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = model * rotateZ * view * projection;

            vao2.On();
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            vao2.Off();
        }

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events.
        glfwPollEvents();
    }

    return 0;
}
