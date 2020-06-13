#include "GLFW/glfw3.h"

#include "Time.h"

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

void UpdateTiming()
{
    float time = glfwGetTime();
    deltaTime = time - lastFrameTime;
    lastFrameTime = time;
}