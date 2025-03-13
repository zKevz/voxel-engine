#pragma once
#include "OpenGL/IndexBuffer.hh"

class VertexArrayObject
{
public:
    void Initialize();

    void Bind() const;
    void Unbind() const;
    void Destroy() const;

    inline constexpr GLuint GetID() const { return m_ID; }

private:
    GLuint m_ID = 0;
};
