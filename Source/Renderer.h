#pragma once 

#include <iostream>
#include <vector>
#include <unordered_map>

#include "VertexArrayObject.h"
#include "Texture.h"
#include "Shader.h"
#include "Containers.h"
#include "Constants.h"
#include "Camera.h"


struct Model
{
    VertexArrayObject& m_VertexArrayObject;
    Shader& m_Shader;
    Texture& m_Texture;
};

class Renderer
{

private:
    GLFWwindow* m_Window;
    Camera* m_Camera = nullptr;
    std::vector<Model> m_Models;

    glm::mat4 m_ModelMatrix = glm::mat4(1.0f);  // All Identity.
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

    float m_DeltaTime = 0.0f;
    float m_LastFrameTime = glfwGetTime();

public:
    Renderer(GLFWwindow* window);
    ~Renderer();
    unsigned int GetItemCount() const;

    Camera* GetCamera() const;    
    void SetCamera(Camera* camera);

    void Push(Model data);
    void Render(float deltaTime);
    void SetUniformMatrix4fv(GLuint& shaderProgram, const char* uniformName, const GLfloat* uniformValue);
    void SetUnifromVector3fv(GLuint& shaderProgram, const char* uniformName, const GLfloat* uniformValue);
};
