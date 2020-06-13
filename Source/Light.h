#pragma once

#include "GLM/vec3.hpp"

class Light
{
    private:
        glm::vec3 m_Position;
        float m_Intensity;

    public:
        virtual glm::vec3 GetPosition() const;
        virtual float GetIntensity() const;
        virtual void SetPosition(glm::vec3 position);
        virtual void SetIntensity(float intensity);
};
