#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GLM/matrix.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "GLM/gtx/string_cast.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "Helpers.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArrayObject.h"
#include "Camera.h"

const char* configPath = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Config/Main.cfg";
// const char* texturePath = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Textures/test_pattern.bmp";
const char* texturePath = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Textures/test_wall.bmp";
const char* shadersPath = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Shaders/Shaders.shd";
const char* shadersPath2 = "/home/lyonbach/Repositories/lbOpenGL/lbOpenGL/Shaders/TextureShader.shd";

// Uniform Variable Names: TODO: Move them to a dedicated constants.
const char* un_mvp = "uMVP";
std::vector<Camera> cameras;
unsigned int cameraCount;

const float vertices[6 * 3] = {
    -0.6f, -0.4f, 0.0f, /*position*/ 1.0f, 0.0f, 0.0f, /*color*/
     0.6f, -0.4f, 0.0f, /*position*/ 0.0f, 1.0f, 0.0f, /*color*/
     0.0f,  0.6f, 0.0f, /*position*/ 0.0f, 0.0f, 1.0f  /*color*/
};

const unsigned int indices[3] = {
    0, 1, 2  // Element numbers.
};

const float vertices2[6 * 4] = {
    -1.0f, -1.0f, 0.0f, /*position*/ 0.0f, 0.0f, 0.0f, /*UVs*/
    -1.0f,  1.0f, 0.0f, /*position*/ 0.0f, 1.0f, 0.0f, /*UVs*/
     1.0f, -1.0f, 0.0f, /*position*/ 1.0f, 0.0f, 0.0f, /*UVs*/
     1.0f,  1.0f, 0.0f, /*position*/ 1.0f, 1.0f, 0.0f, /*UVs*/
};

const unsigned int indices2[3 * 2] = {
    0, 2, 3,  // Element numbers.
    0, 3, 1
};


void ComputeView(GLFWwindow* window, glm::vec3* position, float deltaTime, glm::mat4* viewMatrix, glm::mat4* projectionMatrix)
{
    // glm::vec3 position(0.0, 0.0, 0.0f);
    float horizontalAngle = 0.0001f;
    float verticalAngle = 0.0f;
    float fov = 45.0f;
    float speed = 3.0f;
    float mouseSpeed = 0.05f;
    int screenWidth, screenHeight;

    // Get mouse position
    double mouseX, mouseY;

    glfwGetCursorPos(window, &mouseX, &mouseY);
    glfwGetWindowSize(window, &screenWidth, &screenHeight);
    // glfwSetCursorPos(window, screenWidth * 0.5, screenHeight * 0.5);

    // Compute new orientation
    horizontalAngle += mouseSpeed * deltaTime * float(mouseX);
    verticalAngle   += mouseSpeed * deltaTime * float(mouseY);

    // Convert from spherical coords to rectangular coords
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle));

    // Right Vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f));

    // Up Vector
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS){
        *position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        *position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        *position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        *position -= right * deltaTime * speed;
    }

    // Projection matrix : 45&deg; Field of View, square view, clipping area : 0.01 unit <-> 100 units
    // *projectionMatrix = glm::perspective(glm::radians(fov), 1.0f, 0.01f, 100.0f);
    // View matrix
    *viewMatrix = glm::lookAt(*position, *position + direction, up);
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
    /*
        To avoid calling glfwGetWindowSize on each relevant function call, within this funciton all of the current cameras
        are looped and informed about the screen size.
        Later Renderer class would be responsible of doing that.
    */
   
   for(int i = 0; i < cameraCount; i++)
   {
        Camera camera = cameras[i];
        camera.UpdateWindowSize();
   }
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

    // Set GLFW Parameters
    glfwSwapInterval(1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // Set GLEW Parameters
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Generate Buffers
    // VertexArrayObject vao(vertices, 3 * 6 * sizeof(float), indices, 3);
    VertexArrayObject vao2(vertices2, 4 * 6 * sizeof(float), indices2, 3 * 2);

    // Generate Textures
    Texture texture(1024, 1024, helpers.loadTexture(texturePath));

    // Create Shaders
    // Shader shader(shadersPath);
    Shader textureShader(shadersPath2);
    GLint mvp_location;

    // Create Camera
    Camera camera(window);
    camera.SetPosition(glm::vec3(-5.0f, 2.0f, -5.0f));
    cameras.push_back(camera);

    float rotationAngle = 0.0f;
    glm::mat4 model, view, projection, mvp;
    model = glm::mat4 (1.0f);

    view = glm::mat4 (1.0f);
    camera.SetTarget(&view, glm::vec3(0.0f));
    projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    // Main Loop
    float deltaTime;
    float time;
    float lastTime = glfwGetTime();

    // Calculate transformations
    glm::mat4 translate = glm::translate( glm::vec3(0.0f, 0.0f, 0.0));
    glm::mat4 rotate = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    model = translate * rotate * scale;
    camera.UpdateProjection(&projection);

    
    while(!glfwWindowShouldClose(window))
    {

        // Adjust Timing
        time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // camera.UpdateView(&view, deltaTime);
        
        // Rendering is done here.
        // Draw call 1.
        {
            mvp = projection * view * model;

            vao2.On();
            textureShader.On();
            mvp_location  = glGetUniformLocation(textureShader.getShaderProgram(), un_mvp);
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            textureShader.Off();
            vao2.Off();
        }


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events.
        glfwPollEvents();
    }

    return 0;
}
