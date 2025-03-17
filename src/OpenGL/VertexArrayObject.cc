#include "VertexArrayObject.hh"

#include "Vertex/CrossHairVertex.hh"
#include "Vertex/SkyBoxVertex.hh"
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
    m_VertexBufferObject = vertexBufferObject;
    m_VertexBufferObject.Build();
    Unbind();
}

template<class T>

void VertexArrayObject<T>::SetVertexBufferObject(VertexBufferObject<T> vertexBufferObject)
{
    m_VertexBufferObject = vertexBufferObject;
}

template class VertexArrayObject<VoxelVertex>;
template class VertexArrayObject<SkyBoxVertex>;
template class VertexArrayObject<CrossHairVertex>;
