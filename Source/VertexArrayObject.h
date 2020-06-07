#pragma once

#include <iostream>
#include <vector>

#include "GL/glew.h"

#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include "Containers.h"

class VertexArrayObject
{
private:
    unsigned int m_Id;
    IndexBufferObject m_Ibo;

private:
    void GenerateBuffer(ModelData& modelData, VertexBufferLayout layout, float* data, unsigned int count);

public:
    VertexArrayObject();
    ~VertexArrayObject();
    void Init(ModelData& modelData);
    void On() const;
    void Off() const;
    unsigned int GetElementCount() const;
};
