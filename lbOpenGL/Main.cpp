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
#include "ModelManager.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include "Camera.h"

const char* configPath = "/home/lyonbach/Repositories/lbOpenGL/Config/Main.cfg";
const char* texturePath = "/home/lyonbach/Repositories/lbOpenGL/Textures/test.bmp";
const char* shadersPath = "/home/lyonbach/Repositories/lbOpenGL/Shaders/Shaders.shd";
const char* shadersPath2 = "/home/lyonbach/Repositories/lbOpenGL/Shaders/TextureShader.shd";
const char* modelPath  = "/home/lyonbach/Repositories/lbOpenGL/Models/Suzanne.obj";

#define display(x) for(auto e: x) std::cout << e <<std::endl

// Uniform Variable Names: TODO: Move them to a dedicated constants.
const char* un_mvp = "uMVP";
std::vector<Camera> cameras;
unsigned int cameraCount;

const float vertices[3 * 4] = {
    -1.0f, -1.0f, 0.0f, /*position*/
    -1.0f,  1.0f, 0.0f, /*position*/
     1.0f, -1.0f, 0.0f, /*position*/
     1.0f,  1.0f, 0.0f, /*position*/
};

const float vertices2[3 * 8] = {
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f
};

const float uvs[4 * 2] = {
    0.0f, 0.0f, /*uvs*/
    0.0f, 1.0f, /*uvs*/
    1.0f, 0.0f, /*uvs*/
    1.0f, 1.0f, /*uvs*/
};

const unsigned int indices[3 * 2] = {
    0, 2, 3,  // Element numbers.
    0, 3, 1
};

const unsigned int indices2[3 * 12] = 
{
    2, 3, 1,
    4, 7, 3,
    8, 5, 7,
    6, 1, 5,
    7, 1, 3,
    4, 6, 8,
    2, 4, 3,
    4, 8, 7,
    8, 6, 5,
    6, 2, 1,
    7, 5, 1,
    4, 2, 6
};

const int verticesInt[3 * 4] = {
    -1, -1,  0, /*position*/
    -1,  2,  0, /*position*/
     2, -1,  0, /*position*/
     4,  4,  0, /*position*/
};

const unsigned int uvsInt[4 * 2] = {
    0, 0, /*uvs*/
    0, 1, /*uvs*/
    1, 0, /*uvs*/
    1, 1, /*uvs*/
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
    glEnable(GL_DEPTH_TEST); glEnable(GL_BLEND);
    glDepthFunc(GL_LESS);

    // Create Shaders
    // Shader textureShader(shadersPath);
    Shader textureShader(shadersPath2);
    GLint mvp_location;

    // Read Model From File
    OBJLoader loader(modelPath);
    ModelData modelData;
    loader.Load(&modelData, true);
    
    // int vbc = modelData.vertexBuffer.size();
    // float vb[vbc];
    // std::copy(modelData.vertexBuffer.begin(), modelData.vertexBuffer.end(), vb);

    int vcbc = modelData.vertexCoordsBuffer.size();
    float vcb[vcbc];
    std::copy(modelData.vertexCoordsBuffer.begin(), modelData.vertexCoordsBuffer.end(), vcb);


    int tcbc = modelData.textureCoordsBuffer.size();
    float tcb[tcbc];
    std::copy(modelData.textureCoordsBuffer.begin(), modelData.textureCoordsBuffer.end(), tcb);

    int vnbc = modelData.vertexNormalsBuffer.size();
    float vnb[vnbc];
    std::copy(modelData.vertexNormalsBuffer.begin(), modelData.vertexNormalsBuffer.end(), vnb);

    int ibec = modelData.indexBuffer.size();
    unsigned int ibe[ibec];
    std::copy(modelData.indexBuffer.begin(), modelData.indexBuffer.end(), ibe);
    
    // Generate Vertex Arrays
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(vao);

    VertexBufferLayout vertexLayout;
    vertexLayout.m_ElementCount = 3;
    vertexLayout.m_Index = 0;  // This should be read from the currently bound shader.

    VertexBufferLayout uvsLayout;
    uvsLayout.m_ElementCount = 2;
    uvsLayout.m_Index = 1;

    VertexBufferLayout normalsLayout;
    normalsLayout.m_ElementCount = 3;
    normalsLayout.m_Index = 2;

    // Create Vertex Coordinates Buffer - Should be handled by a model class later.
    VertexBufferObject vertexCoordsBufferObject(vcb, vcbc);
    vertexCoordsBufferObject.SetLayout(vertexLayout);
    vertexCoordsBufferObject.On();
    glEnableVertexAttribArray(vertexCoordsBufferObject.GetLayoutIndex());
    glVertexAttribPointer(vertexCoordsBufferObject.GetLayoutIndex(),
                          vertexCoordsBufferObject.GetLayoutElementCount(),
                          vertexCoordsBufferObject.GetDataType(),
                          vertexCoordsBufferObject.IsNormalized(),
                          vertexCoordsBufferObject.GetStride(),
                          vertexCoordsBufferObject.GetOffsetPointer());
    vertexCoordsBufferObject.Off();

    // Create Texture Coordinates Buffer
    VertexBufferObject textureCoordsBufferObject(tcb, tcbc);
    textureCoordsBufferObject.SetLayout(uvsLayout);
    textureCoordsBufferObject.On();
    glEnableVertexAttribArray(textureCoordsBufferObject.GetLayoutIndex());
    glVertexAttribPointer(textureCoordsBufferObject.GetLayoutIndex(),
                          textureCoordsBufferObject.GetLayoutElementCount(),
                          textureCoordsBufferObject.GetDataType(),
                          textureCoordsBufferObject.IsNormalized(),
                          textureCoordsBufferObject.GetStride(),
                          textureCoordsBufferObject.GetOffsetPointer());
    textureCoordsBufferObject.Off();

    // Create Vertex Normals Buffer
    VertexBufferObject vertexNormalsBufferObject(vnb, vnbc);
    vertexNormalsBufferObject.SetLayout(normalsLayout);
    vertexNormalsBufferObject.On();
    glEnableVertexAttribArray(vertexNormalsBufferObject.GetLayoutIndex());
    glVertexAttribPointer(vertexNormalsBufferObject.GetLayoutIndex(),
                          vertexNormalsBufferObject.GetLayoutElementCount(),
                          vertexNormalsBufferObject.GetDataType(),
                          vertexNormalsBufferObject.IsNormalized(),
                          vertexNormalsBufferObject.GetStride(),
                          vertexNormalsBufferObject.GetOffsetPointer());
    vertexNormalsBufferObject.Off();

    // Generate Index Buffers
    IndexBufferObject ibo(ibe, ibec);

    // Generate Textures
    Texture texture(1024, 1024, helpers.loadTexture(texturePath));

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
    glm::mat4 rotate = glm::rotate(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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

            glBindVertexArray(vao);
            ibo.On();

            textureShader.On();
            mvp_location  = glGetUniformLocation(textureShader.getShaderProgram(), un_mvp);
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
            glDrawElements(GL_TRIANGLES, ibo.GetElementCount(), GL_UNSIGNED_INT, 0);
            // glDrawArrays(GL_TRIANGLES, 0, sizeof(vcbc + tcbc + vnbc) / sizeof(float) / 8);
            textureShader.Off();
        }


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events.
        glfwPollEvents();
    }

    return 0;
}
