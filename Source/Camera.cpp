#include <iostream>
#include "Camera.h"

// Camera::Camera(GLFWwindow* window, glm::vec3 position, glm::vec3 target, float clipNear, float clipFar)
//     : m_Window(window), m_Position(position), m_FrontVector(target), m_ClipNear(clipNear), m_ClipFar(clipFar)
Camera::Camera(GLFWwindow* window)
    : m_Window(window)
{
    UpdateWindowSize();
}

Camera::~Camera()
{
}

glm::vec3 Camera::GetPosition() const
{
    return m_Position;
}

void Camera::UpdateWindowSize()
{
    glfwGetWindowSize(m_Window, &m_ScreenWidth, &m_ScreenHeight);
    m_ScreenRatio = (float)m_ScreenWidth / (float)m_ScreenHeight;
    std::cout << "[DEBUG][CAMERA]: Screen size updated." << std::endl;
}

void Camera::UpdateProjection(glm::mat4* projection)
{
    // Calculate screen ratio.
    int width, height;
    *projection = glm::perspective(glm::radians(m_FOV), m_ScreenRatio, m_ClipNear, m_ClipFar);
    std::cout << "[DEBUG][CAMERA]: Projection matrix updated." << std::endl;
}

void Camera::UpdateView(glm::mat4* view, float deltaTime)
{
    float speed = 6.0f;  // TODO: Should be moved to a constants or configuration struct.
    float mouseSpeed = 0.05f;  // TODO: Should be moved to a constants or configuration struct.
    float horizontalAngle = 0.0f;  //TODO: Get this from current
    float verticalAngle = 0.0f;

    // Truncate deltaTime to avoid noise.
    deltaTime = std::trunc(deltaTime*100) / 100;

    // Heading.
    // Update cursor position.
    double mouseX, mouseY;
    glfwGetCursorPos(m_Window, &mouseX, &mouseY);
    // glfwSetCursorPos(m_Window, m_ScreenWidth/2, m_ScreenHeight/2);
    // Update orientations.
    horizontalAngle -= mouseSpeed * deltaTime * float(mouseX);
    verticalAngle   += mouseSpeed * deltaTime * float(mouseY);

    // Convert from spherical coords to rectangular coordinates.
    m_FrontVector = glm::vec3(cos(verticalAngle) * sin(horizontalAngle),
                              sin(verticalAngle),
                              cos(verticalAngle) * cos(horizontalAngle));

    // Right Vector
    m_RightVector = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f));


    // Orientation
    // Spin CCW
    if (glfwGetKey(m_Window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        // B = A - (A.dor.N)N,
        // Reflect right vector to the horizontal worldspace and subtract the result from the original vector.
        // glm::vec3 normalWS(0.0f, 1.0f, 0.0f);
        // glm::vec3 projected = m_RightVector - glm::dot(m_RightVector, normalWS) * normalWS;
        // std::cout << glm::to_string(projected) << std::endl;
        // float cosTheta = glm::dot(m_RightVector, projected) / (glm::length(m_RightVector) * glm::length(projected));
        // cosTheta += speed * deltaTime;
        // m_RightVector = glm::rotate(m_RightVector, glm::radians(cosTheta), m_FrontVector);
        // std::cout << "[DEBUG][CAMERA]: Implement Spinning!" << std::endl;
        SetTarget(glm::vec3(2.0, 0.0, 0.0));
    }

    // Spin CW
    if (glfwGetKey(m_Window, GLFW_KEY_C) == GLFW_PRESS)
    {
        std::cout << "[DEBUG][CAMERA]: Implement Spinning!" << std::endl;
    }

    // Up Vector
    m_UpVector = glm::cross(m_RightVector, m_FrontVector);

    // Position.
    // Move forward
    if (glfwGetKey(m_Window, GLFW_KEY_W ) == GLFW_PRESS){
        m_Position += m_FrontVector * deltaTime * speed;
    }

    // Move backward
    if (glfwGetKey(m_Window, GLFW_KEY_S ) == GLFW_PRESS){
        m_Position -= m_FrontVector * deltaTime * speed;
    }

    // Strafe right
    if (glfwGetKey(m_Window, GLFW_KEY_D ) == GLFW_PRESS){
        m_Position += m_RightVector * deltaTime * speed;
    }

    // Strafe left
    if (glfwGetKey(m_Window, GLFW_KEY_A ) == GLFW_PRESS){
        m_Position -= m_RightVector * deltaTime * speed;
    }

    // Climb
    if (glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_Position += m_UpVector * deltaTime * speed;
    }

    // Dive
    if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_Position -= m_UpVector * deltaTime * speed;
    }

    // View matrix
    *view = glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

void Camera::SetPosition(glm::vec3 position)
{
    m_Position = position;
    std::cout << "[DEBUG][CAMERA]: Position has been set to\n\t" << glm::to_string(position) << std::endl;
}

void Camera::SetTarget(glm::vec3 target)
{
    m_FrontVector = target;
    std::cout << "[DEBUG][CAMERA]: Target has been set to\n\t" << glm::to_string(target) << std::endl;
}

void Camera::SetZoom(float zoom)
{
    m_ZoomLevel = zoom;
    m_FOV *= 1 / m_ZoomLevel;
    SetFOV(m_FOV);
    std::cout << "[DEBUG][CAMERA]: Zoom level updated." << std::endl;
}

void Camera::SetFOV(float FOV)
{
    m_FOV = FOV;
}

void Camera::SetNearClip(float clipNear)
{
    m_ClipNear = clipNear;
}

void Camera::SetFarClip(float clipFar)
{
    m_ClipFar = clipFar;
}
