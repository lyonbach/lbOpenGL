#pragma once

#include "GL/glew.h"

class IndexBufferObject
{
private:
    unsigned int m_Id;
    const void* m_Data;
    unsigned int m_Count;
    int m_Usage = GL_STATIC_DRAW;
    int m_dType = GL_UNSIGNED_INT;

private:
    int GetSize() const;
    int GetUsage() const;
    void SetUsage(int usage);

public:
    ~IndexBufferObject();
    unsigned int GetElementCount() const;
    void Init(const void* data, unsigned int count);
    void On() const;
    void Off() const;
};
