#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GLM/vec3.hpp"
#include "GLM/mat4x4.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "GLM/ext/matrix_clip_space.hpp"
#include "GLM/gtx/string_cast.hpp"

class Camera
{
private:
    GLFWwindow* m_Window;
 
    // unsigned int m_Id;
    glm::vec3 m_Position = glm::vec3(0.0f);
    // glm::quat m_Rotation;  // TODO: Implement as quaternion.
    // glm::vec3 m_Target;
    glm::vec3 m_FrontVector = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 m_UpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_RightVector = glm::vec3(-1.0f, 0.0, 0.0);
    float m_FOV = 45.0f;
    float m_ZoomLevel = 1.0f; // 2.0 means 2x zoom -> FOV= 45.0f / 2
    float m_ClipNear = 0.1f;
    float m_ClipFar = 100.0f;
    int m_ScreenWidth = 0;
    int m_ScreenHeight = 0;
    float m_ScreenRatio = 1.0f;

public:
    // Camera(GLFWwindow* window, glm::vec3 position, glm::vec3 target, float clipNear, float clipFar);
    Camera(GLFWwindow* window);
    ~Camera();
 
    glm::vec3 GetPosition() const;

    void UpdateProjection(glm::mat4* projection);
    void UpdateView(glm::mat4* view, float deltaTime);

    void SetPosition(glm::vec3 position);
    void SetTarget(glm::vec3 target);
    void SetZoom(float zoom);
    void SetNearClip(float clipNear);
    void SetFarClip(float clipFar);
    void UpdateWindowSize();

private:
    void SetFOV(float FOV);


};
