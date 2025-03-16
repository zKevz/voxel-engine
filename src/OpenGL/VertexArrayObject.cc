#include "VertexArrayObject.hh"

#include "Vertex/CrossHairVertex.hh"
#include "Vertex/VoxelVertex.hh"

template<class T>
void VertexArrayObject<T>::Initialize()
{
    glGenVertexArrays(1, &m_ID);
}

template<class T>
void VertexArrayObject<T>::Bind() const
{
    glBindVertexArray(m_ID);
}

template<class T>
void VertexArrayObject<T>::Unbind() const
{
    glBindVertexArray(0);
}

template<class T>
void VertexArrayObject<T>::Destroy() const
{
    DestroyInner();
    glDeleteVertexArrays(1, &m_ID);
}

template<class T>
void VertexArrayObject<T>::DestroyInner() const
{
    m_IndexBuffer.Destroy();
    m_VertexBufferObject.Destroy();
}

template<class T>
void VertexArrayObject<T>::Set(VertexBufferObject<T> vertexBufferObject, std::vector<GLuint> indices)
{
    Bind();
    m_IndexBuffer.Initialize(indices);
    vertexBufferObject.Build();
    Unbind();
}

template class VertexArrayObject<VoxelVertex>;
template class VertexArrayObject<CrossHairVertex>;
