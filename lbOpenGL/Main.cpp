#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GLM/matrix.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "GLM/gtx/string_cast.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "Definitions.h"

#include "Helpers.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include "ModelManager.h"

#include "Texture.h"
#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"

const char* configPath = "/home/lyonbach/Repositories/lbOpenGL/Config/Main.cfg";


#define display(x) for(auto e: x) std::cout << e <<std::endl

// Uniform Variable Names: TODO: Move them to a dedicated constants.

std::vector<Camera> cameras;
unsigned int cameraCount;

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
        To avoid calling glfwGetWindowSize on each relevant function call,
        all of the current cameras are looped and informed about the screen size.
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

    std::cout << "[INFO][MAIN]: Starting: \"" << windowName << "\"" << std::endl;

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "[ERROR][MAIN]: Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // Set GLFW Parameters
    glfwSwapInterval(1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create Window
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Set Window Parameters
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
    glEnable(GL_CULL_FACE);


    // ####################################################################################################
    // ### SETUP OBJECTS ##################################################################################
    // ####################################################################################################

    // Create containers that will live during the gameplay. (Each one is 1 in lenght for testing purposes.)
    VertexArrayObject vertexArrayObjects[1];  // Create the container for scene object data.
    Texture textures[1];                      // Create container for textures.
    Shader shaders[1];                        // Create container for shaders.
    
    // Load Models
    ModelManager modelManager;
    modelManager.Push(config.m_modelPath.c_str());
    modelManager.Load();

    // Populate scene object data container
    for(int i = 0; i < modelManager.GetModelDataArray().size(); i++)
    {
        ModelData *md = modelManager.GetModelDataArray().at(i);
        vertexArrayObjects[i].Init(*md);
    }

    
    // Load & Generate Textures
    Texture texture;
    texture.SetTexturePath(config.m_texturePath.c_str());
    texture.LoadTexture();
    textures[0] = texture;

    // Create Shaders
    Shader shader;
    shader.SetShaderFilePath(config.m_shadersFilePath.c_str());
    shader.GenerateShaders();
    shaders[0] = shader;

    // Cleanup
    // modelManager.~ModelManager();

    // Create Camera
    Camera camera(window);
    camera.SetPosition(glm::vec3(0.0f, 0.0f, -6.0f));
    cameras.push_back(camera);

    // Setup Renderer
    Renderer renderer(window);
    renderer.Push({vertexArrayObjects[0], shaders[0], textures[0]});
    renderer.SetCamera(&camera);

    // float rotationAngle = 0.0f;
    // glm::mat4 model, view, projection, mvp;
    // model = glm::mat4 (1.0f);

    // view = glm::mat4 (1.0f);
    // camera.SetTarget(&view, glm::vec3(0.0f));

    float deltaTime;
    float time;
    float lastTime = glfwGetTime();

    // camera.UpdateProjection(&projection);
    // Calculate transformations
    // glm::mat4 translate;
    // glm::mat4 rotate;
    // glm::mat4 scale;

    // float someVal = 5.0f;
    // glm::vec3 lightPos(2.0f, 2.0f, -2.0f);
    // glm::vec3 camPos(0.0f);

    // GLint m_location, v_location, p_location, lp_location, cp_location;
    // Main Loop
    while(!glfwWindowShouldClose(window))
    {

        // Adjust Timing
        time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // camera.UpdateView(&view, deltaTime);

        // Update light position
        // someVal += deltaTime / 2;

        renderer.Render(deltaTime);
        // Update camera position
        // camPos = camera.GetPosition();

        // translate = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
        // rotate = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // scale = glm::scale(glm::vec3(1.0f));
        // model = translate * rotate * scale;

        // m_location  = glGetUniformLocation(shader.GetShaderProgram(), uniform_model_string);
        // v_location  = glGetUniformLocation(shader.GetShaderProgram(), uniform_view_string);
        // p_location  = glGetUniformLocation(shader.GetShaderProgram(), uniform_projection_string);
        // lp_location = glGetUniformLocation(shader.GetShaderProgram(), uniform_lightPositionWS);
        // cp_location = glGetUniformLocation(shader.GetShaderProgram(), uniform_cameraPositionWS);

        // glUniformMatrix4fv(m_location, 1, GL_FALSE, &model[0][0]);
        // glUniformMatrix4fv(v_location, 1, GL_FALSE, &view[0][0]);
        // glUniformMatrix4fv(p_location, 1, GL_FALSE, &projection[0][0]);
        // glUniform3fv(lp_location, 1, &lightPos[0]);
        // glUniform3fv(cp_location, 1, &camPos[0]);

        // vertexArrayObjects[0].On();
        // glDrawElements(GL_TRIANGLES, vertexArrayObjects[0].GetElementCount(), GL_UNSIGNED_INT, 0);
        // vertexArrayObjects[0].Off();


        // shader.Off();
        // Swap front and back buffers
        // glfwSwapBuffers(window);

        // Poll for and process events.
        // glfwPollEvents();
    }

    return 0;
}
