#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{

}

void VertexArrayObject::Init(ModelData& modelData)
{
    // Generate Vertex Array
    glGenVertexArrays(1, &m_Id);
    glBindVertexArray(m_Id);
    glEnableVertexAttribArray(m_Id);

    // Generate Buffers
    int vcbc = modelData.vertexCoordsBuffer.size();
    float vcb[vcbc];
    std::copy(modelData.vertexCoordsBuffer.begin(), modelData.vertexCoordsBuffer.end(), vcb);

    VertexBufferLayout vertexBufferLayout;
    vertexBufferLayout.m_ElementCount = 3;
    vertexBufferLayout.m_Index = 0;  // This should be read from the currently bound shader.
    GenerateBuffer(modelData, vertexBufferLayout, vcb, vcbc);

    int tcbc = modelData.textureCoordsBuffer.size();
    float tcb[tcbc];
    std::copy(modelData.textureCoordsBuffer.begin(), modelData.textureCoordsBuffer.end(), tcb);

    VertexBufferLayout textureBufferLayout;
    textureBufferLayout.m_ElementCount = 2;
    textureBufferLayout.m_Index = 1;
    GenerateBuffer(modelData, textureBufferLayout, tcb, tcbc);

    int vnbc = modelData.vertexNormalsBuffer.size();
    float vnb[vnbc];
    std::copy(modelData.vertexNormalsBuffer.begin(), modelData.vertexNormalsBuffer.end(), vnb);

    VertexBufferLayout normalsBufferLayout;
    normalsBufferLayout.m_ElementCount = 3;
    normalsBufferLayout.m_Index = 2;
    GenerateBuffer(modelData, normalsBufferLayout, vnb, vnbc);
    int ibec = modelData.indexBuffer.size();
    unsigned int ibe[ibec];
    std::copy(modelData.indexBuffer.begin(), modelData.indexBuffer.end(), ibe);
    m_Ibo.Init(ibe, ibec);

    glBindVertexArray(0);
}

VertexArrayObject::~VertexArrayObject()
{
}

void VertexArrayObject::GenerateBuffer(ModelData& modelData, VertexBufferLayout layout, float* data, unsigned int count)
{

    VertexBufferObject vbo(data, count);
    vbo.SetLayout(layout);
    vbo.On();
    glEnableVertexAttribArray(vbo.GetLayoutIndex());
    glVertexAttribPointer(vbo.GetLayoutIndex(),
                          vbo.GetLayoutElementCount(),
                          vbo.GetDataType(),
                          vbo.IsNormalized(),
                          vbo.GetStride(),
                          vbo.GetOffsetPointer());
    vbo.Off();
}

void VertexArrayObject::On() const
{
    glBindVertexArray(m_Id);
    m_Ibo.On();
}

void VertexArrayObject::Off() const
{
    glBindVertexArray(0);
    m_Ibo.Off();
}

unsigned int VertexArrayObject::GetElementCount() const
{
    return m_Ibo.GetElementCount();
}