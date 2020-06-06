#include "IndexBufferObject.h"


IndexBufferObject::IndexBufferObject(const void* data, unsigned int count)
    : m_Data(data), m_Count(count)
{
    glGenBuffers(1, &m_Id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetSize(), m_Data, m_Usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBufferObject::~IndexBufferObject()
{
    Off();
    glDeleteBuffers(1, &m_Id);
}

void IndexBufferObject::On() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void IndexBufferObject::Off() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int IndexBufferObject::GetSize() const
{
    return sizeof(m_dType)* m_Count;
}

int IndexBufferObject::GetUsage() const
{
    return m_Usage;
}

unsigned int IndexBufferObject::GetElementCount() const
{
    return m_Count;
}