#pragma once

#include "Light.h"

class PointLight : public Light
{
private:
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    unsigned int m_Intensity = 1.0;

public:
    virtual glm::vec3 GetPosition() const;
    virtual float GetIntensity() const;
    virtual void SetPosition(glm::vec3 position);
    virtual void SetIntensity(float intensity);
};
