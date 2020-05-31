#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject(const void* vertexData,
                                     unsigned int vertexCount,
                                     const void* indexData,
                                     unsigned int indexCount)
{
    // Generate Vertex Array Object
    glGenVertexArrays(1, &m_VertexArrayObjectId);
    glBindVertexArray(m_VertexArrayObjectId);
    glEnableVertexAttribArray(m_VertexArrayObjectId);

    // Generate Vertex Buffer Object
    glGenBuffers(1, &m_VertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertexData, GL_STATIC_DRAW);

    /* 
        Here this part is pretty much hardcoded. There must be an additional object to provide the specifications
        for how the data should be interpretted. e.g. VertexBufferLayout
    */

    // Generate Index Buffer Object
    glGenBuffers(1, &m_IndexBufferObjectId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObjectId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indexData, GL_STATIC_DRAW);



    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*) (sizeof(float) * 3));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}


VertexArrayObject::~VertexArrayObject()
{
    Off();
    glDeleteBuffers(1, &m_VertexBufferObjectId);
    glDeleteBuffers(1, &m_IndexBufferObjectId);
    glDeleteVertexArrays(1, &m_VertexBufferObjectId);
}


void VertexArrayObject::On() const
{
    glBindVertexArray(m_VertexArrayObjectId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjectId);
}

void VertexArrayObject::Off() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
