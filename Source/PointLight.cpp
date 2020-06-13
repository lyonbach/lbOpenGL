#include "PointLight.h"

glm::vec3 PointLight::GetPosition() const
{
    return m_Position;
}

float PointLight::GetIntensity() const
{
    return m_Intensity;
}

void PointLight::SetPosition(glm::vec3 position)
{
    m_Position = position;
}

void PointLight::SetIntensity(float intensity)
{
    m_Intensity = intensity;
}