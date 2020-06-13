#include "Renderer.h"


Renderer::Renderer(GLFWwindow* window)
    : m_Window(window)
{
}

Renderer::~Renderer()
{
    m_Models.clear();
    m_Models.shrink_to_fit();
}

Camera* Renderer::GetCamera() const
{
    return m_Camera;
}

unsigned int Renderer::GetItemCount() const
{
    return m_Models.size();
}

void Renderer::SetCamera(Camera* camera)
{
    m_Camera = camera;
    m_Camera->UpdateProjection(&m_ProjectionMatrix);
}

void Renderer::Push(Model data)
{
    m_Models.push_back(data);
}

void Renderer::Render(float deltaTime)
{

    if(m_Camera == nullptr)
    {
        std::cout << "[ERROR][RENDERER]: Can not render without a camera!" << std::endl;
        std::exit(1);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Adjust timing
    m_DeltaTime = deltaTime;

    // Here all vertex array data, index buffer data, vertex buffer data, shaders, textures that needs to be included is 
    // being prepared.
    unsigned int modelCount = m_Models.size();
   
    // std::cout << "[DEBUG][RENDERER]: Preparing frame." << std::endl;
    // std::cout << "[DEBUG][RENDERER]: Total " << modelCount << " object(s) will be rendered." << std::endl;
   
    m_Camera->UpdateView(&m_ViewMatrix, m_DeltaTime);
    // translate = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    // rotate = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // scale = glm::scale(glm::vec3(1.0f));
    // model = translate * rotate * scale;

    for(int i = 0; i < modelCount; i++)
    {
        Model model = m_Models.at(i);
        GLuint shaderProgram = model.m_Shader.GetShaderProgram();
        model.m_VertexArrayObject.On();
        model.m_Texture.On();
        model.m_Shader.On();

        SetUniformMatrix4fv(shaderProgram, uniform_model_string, &m_ModelMatrix[0][0]);
        SetUniformMatrix4fv(shaderProgram, uniform_view_string, &m_ViewMatrix[0][0]);
        SetUniformMatrix4fv(shaderProgram, uniform_projection_string, &m_ProjectionMatrix[0][0]);

        SetUnifromVector3fv(shaderProgram, uniform_lightPositionWS, &model.m_Light.GetPosition()[0]);
        // TODO: Instead of setting the uniform get inverse of the view matrix within the shader
        SetUnifromVector3fv(shaderProgram, uniform_cameraPositionWS, &m_Camera->GetPosition()[0]);
        SetUniformScalar1f(shaderProgram, uniform_lightIntensity, model.m_Light.GetIntensity());

        glDrawElements(GL_TRIANGLES, model.m_VertexArrayObject.GetElementCount(), GL_UNSIGNED_INT, 0);

        model.m_VertexArrayObject.Off();
        model.m_Texture.Off();
        model.m_Shader.Off();

        // Swap front and back buffers
        glfwSwapBuffers(m_Window);

        // Poll for and process events.
        glfwPollEvents();
    }
}

void Renderer::SetUniformMatrix4fv(GLuint& shaderProgram, const char* uniformName, const GLfloat* uniformValue)
{
    GLuint uniformLocation = glGetUniformLocation(shaderProgram, uniformName);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, uniformValue);
}

void Renderer::SetUnifromVector3fv(GLuint& shaderProgram, const char* uniformName, const GLfloat* uniformValue)
{
    GLuint uniformLocation = glGetUniformLocation(shaderProgram, uniformName);
    glUniform3fv(uniformLocation, 1, &uniformValue[0]);
}

void Renderer::SetUniformScalar1f(GLuint& shaderProgram, const char* uniformName, const GLfloat uniformValue)
{
    GLuint uniformLocation = glGetUniformLocation(shaderProgram, uniformName);
    glUniform1f(uniformLocation, uniformValue);
}