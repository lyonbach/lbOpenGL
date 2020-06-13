#pragma once 

#include <vector>
#include <unordered_map>

#include "VertexArrayObject.h"
#include "Texture.h"
#include "Shader.h"
#include "Containers.h"


struct Model
{
    VertexArrayObject* m_VertexArrayObject;
    Shader* m_Shader;
    Texture* m_Texture;
};


class Renderer
{

private:
    std::vector<Model> m_Models;

public:
    Renderer();
    ~Renderer();
    unsigned int GetItemCount() const;
    void Push(Model data);
};

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
    m_Models.clear();
    m_Models.shrink_to_fit();
}

unsigned int Renderer::GetItemCount() const
{
    return m_Models.size();
}

void Renderer::Push(Model data)
{
    m_Models.push_back(data);
}

