#include "VertexArrayObject.hh"

void VertexArrayObject::Initialize()
{
    glGenVertexArrays(1, &m_ID);
}

void VertexArrayObject::Bind() const
{
    glBindVertexArray(m_ID);
}

void VertexArrayObject::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArrayObject::Destroy() const
{
    glDeleteVertexArrays(1, &m_ID);
}
