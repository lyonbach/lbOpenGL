#pragma once
#include <iostream>

#include "GL/glew.h"

class VertexArrayObject
{
private:
    GLuint m_VertexArrayObjectId;
    GLuint m_VertexBufferObjectId;
    GLuint m_IndexBufferObjectId;

public:
    VertexArrayObject(const void* vertexData,
                      unsigned int vertexCount,
                      const void* indexData,
                      unsigned int indexCount);
    ~VertexArrayObject();
    void On() const;
    void Off() const;
};
