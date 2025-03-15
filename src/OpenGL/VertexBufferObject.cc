#include "VertexBufferObject.hh"

#include "Chunk.hh"
#include "OpenGL/VoxelVertex.hh"

template<class T>
VertexBufferObject<T>::VertexBufferObject()
{
    m_Vertex.reserve(kChunkWidth * kChunkHeight * kChunkDepth * 24);
}

template<class T>
void VertexBufferObject<T>::Initialize()
{
    glGenBuffers(1, &m_ID);
}

template<class T>
void VertexBufferObject<T>::AddByte(GLuint count)
{
    m_Layout.push_back(VertexBufferLayout { GL_UNSIGNED_BYTE, count, GL_TRUE });
}

template<class T>
void VertexBufferObject<T>::AddFloat(GLuint count)
{
    m_Layout.push_back(VertexBufferLayout { GL_FLOAT, count, GL_FALSE });
}

template<class T>
void VertexBufferObject<T>::AddUInteger(GLuint count)
{
    m_Layout.push_back(VertexBufferLayout { GL_UNSIGNED_INT, count, GL_FALSE });
}

template<class T>
void VertexBufferObject<T>::AddVertex(T vertex)
{
    m_Vertex.emplace_back(std::move(vertex));
}

template<class T>
void VertexBufferObject<T>::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

template<class T>
void VertexBufferObject<T>::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<class T>
void VertexBufferObject<T>::Destroy() const
{
    glDeleteBuffers(1, &m_ID);
}

template<class T>
void VertexBufferObject<T>::Build() const
{
    Bind();
    glBufferData(GL_ARRAY_BUFFER, m_Vertex.size() * sizeof(T), m_Vertex.data(), GL_STATIC_DRAW);

    GLuint offset = 0;

    for (unsigned int i = 0; i < m_Layout.size(); ++i)
    {
        const VertexBufferLayout &layout = m_Layout[i];

        glVertexAttribPointer(i, layout.Count, layout.Type, layout.Normalized, sizeof(T), (void *) (uintptr_t) offset);
        glEnableVertexAttribArray(i);

        offset += layout.Count * layout.GetSize();
    }

    Unbind();
}

template class VertexBufferObject<VoxelVertex>;
