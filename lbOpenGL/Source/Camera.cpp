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
    // void ComputeView(glm::mat4* viewMatrix)
    float speed = 3.0f;  // TODO: Should be moved to a constants or configuration struct.
    float mouseSpeed = 0.05f;  // TODO: Should be moved to a constants or configuration struct.
    float horizontalAngle = 0.0f;
    float verticalAngle = 0.0f;

    // Truncate deltaTime to avoid noise.
    deltaTime = std::trunc(deltaTime*100) / 100;

    // Heading.
    // Update cursor position.
    double mouseX, mouseY;
    glfwGetCursorPos(m_Window, &mouseX, &mouseY);

    // Update orientations.
    horizontalAngle -= mouseSpeed * deltaTime * float(mouseX);
    verticalAngle   += mouseSpeed * deltaTime * float(mouseY);
    // Convert from spherical coords to rectangular coordinates.
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle));

    // std::cout << "mouseX: " << mouseX << std::endl;
    // std::cout << "mouseY: " << mouseY << std::endl;
    // std::cout << "deltaTime: " << deltaTime << std::endl;  

    // Right Vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f));

    // Up Vector
    glm::vec3 up = glm::cross(right, direction);

    // Position.
        // Move forward
    if (glfwGetKey(m_Window, GLFW_KEY_W ) == GLFW_PRESS){
        m_Position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(m_Window, GLFW_KEY_S ) == GLFW_PRESS){
        m_Position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(m_Window, GLFW_KEY_D ) == GLFW_PRESS){
        m_Position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(m_Window, GLFW_KEY_A ) == GLFW_PRESS){
        m_Position -= right * deltaTime * speed;
    }
    // Climb
    if (glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_Position += up * deltaTime * speed;
    }
    // Dive
    if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_Position -= up * deltaTime * speed;
    }


    // Projection matrix : 45&deg; Field of View, square view, clipping area : 0.01 unit <-> 100 units
    // *projectionMatrix = glm::perspective(glm::radians(fov), 1.0f, 0.01f, 100.0f);
    // View matrix
    *view = glm::lookAt(m_Position, m_Position + direction, up);
    // glfwSetCursorPos(m_Window, double(m_ScreenWidth)/2, double(m_ScreenHeight)/2);
}

void Camera::SetPosition(glm::vec3 position)
{
    m_Position = position;
    std::cout << "[DEBUG][CAMERA]: Target has been set to\n\t" << glm::to_string(position) << std::endl;
}

void Camera::SetTarget(glm::mat4* view, glm::vec3 target)
{
    m_FrontVector = target;
    *view = glm::lookAt(
        m_Position,
        m_FrontVector,
        m_UpVector
        );
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
