#include "IndexBuffer.hh"

#include <vector>

void IndexBuffer::Set(std::vector<GLuint> indices)
{
    static_assert(sizeof(unsigned int) == sizeof(GLuint));

    m_Count = indices.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);
}

void IndexBuffer::Replace(std::vector<GLuint> indices)
{
    m_Count = indices.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_Count * sizeof(GLuint), indices.data());
}

void IndexBuffer::Initialize(std::vector<GLuint> indices)
{
    glGenBuffers(1, &m_ID);
    Set(indices);
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
