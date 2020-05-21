#include <iostream>

#include "GLFW/glfw3.h"

 static void cb_KeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


int main(int argc, char const *argv[])
{

    const char* windowName = "Simple Window";
    const int windowWidth = 640;
    const int windowHeight = 480;

    std::cout << "Testing 1, 2, 3..." << std::endl;

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
    

    // Main Loop
    while(!glfwWindowShouldClose(window))
    {
        
        // Render is done here.
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events.
        glfwPollEvents();
    }

    return 0;
}
