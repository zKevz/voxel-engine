#include "IndexBuffer.hh"

#include <vector>

void IndexBuffer::Initialize(std::vector<GLuint> indices)
{
    static_assert(sizeof(unsigned int) == sizeof(GLuint));

    m_Count = indices.size();

    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Destroy() const
{
    glDeleteBuffers(1, &m_ID);
}
