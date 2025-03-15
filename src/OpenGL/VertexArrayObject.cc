#include "VertexArrayObject.hh"

#include "OpenGL/VoxelVertex.hh"

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
    m_IndexBuffer.Destroy();
    m_VertexBufferObject.Destroy();

    glDeleteVertexArrays(1, &m_ID);
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
