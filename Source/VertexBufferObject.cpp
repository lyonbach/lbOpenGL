#include "VertexBufferObject.h"
#include <vector>

VertexBufferObject::VertexBufferObject(const void* data, unsigned int count)
    : m_Data(data), m_Count(count)
{
    glGenBuffers(1, &m_Id);
    glBindBuffer(GL_ARRAY_BUFFER, m_Id);
    glBufferData(GL_ARRAY_BUFFER, GetSize(), m_Data, m_Usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBufferObject::~VertexBufferObject()
{
    // Off();
    // glDeleteBuffers(1, &m_Id);
}

void VertexBufferObject::On() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Id);
}

void VertexBufferObject::Off() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int VertexBufferObject::GetSize() const
{
    return sizeof(m_VertexBufferLayout.m_Type)* m_Count;
}

int VertexBufferObject::GetUsage() const
{
    return m_Usage;
}

unsigned int VertexBufferObject::GetLayoutIndex() const
{
    return m_VertexBufferLayout.m_Index;
}

int VertexBufferObject::GetLayoutElementCount() const
{
    return m_VertexBufferLayout.m_ElementCount;
}

int VertexBufferObject::GetDataType() const
{
    return m_VertexBufferLayout.m_Type;
}

int VertexBufferObject::IsNormalized() const
{
    return m_VertexBufferLayout.m_Normalized;
}

int VertexBufferObject::GetStride() const
{
    return sizeof(m_VertexBufferLayout.m_Type) * m_VertexBufferLayout.m_ElementCount;
}

const void* VertexBufferObject::GetOffsetPointer() const
{
    return m_VertexBufferLayout.m_Pointer;
}

void VertexBufferObject::SetLayout(VertexBufferLayout layout)
{
    m_VertexBufferLayout = layout;
}