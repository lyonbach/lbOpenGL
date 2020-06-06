#pragma once

#include "GL/glew.h"

struct VertexBufferLayout
{
    unsigned int m_Index;
    int m_ElementCount;
    int m_Type = GL_FLOAT;
    int m_Normalized = GL_FALSE;
    const void* m_Pointer = 0;
};

class VertexBufferObject
{
private:
    VertexBufferLayout m_VertexBufferLayout;
    unsigned int m_Id;
    const void* m_Data;
    unsigned int m_Count;
    int m_Usage = GL_STATIC_DRAW;

private:
    int GetSize() const;
    int GetUsage() const;
    void SetUsage(int usage);

public:
    VertexBufferObject(const void* data, unsigned int count);
    ~VertexBufferObject();
    void On() const;
    void Off() const;
    unsigned int GetLayoutIndex() const;
    int GetLayoutElementCount() const;
    int GetDataType() const;
    int IsNormalized() const;
    int GetStride() const;
    const void* GetOffsetPointer() const;

    void SetLayout(VertexBufferLayout layout);

};
    