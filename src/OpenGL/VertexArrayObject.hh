#pragma once
#include "OpenGL/IndexBuffer.hh"
#include "OpenGL/VertexBufferObject.hh"

template<class T>
class VertexArrayObject
{
public:
    void Initialize();

    void Bind() const;
    void Unbind() const;
    void Destroy() const;

    void Set(VertexBufferObject<T> vertexBufferObject, std::vector<GLuint> indices);

    inline constexpr GLuint GetID() const
    {
        return m_ID;
    }

    inline constexpr GLuint GetIndicesCount() const
    {
        return m_IndexBuffer.GetCount();
    }

private:
    GLuint m_ID = 0;

    IndexBuffer m_IndexBuffer;
    VertexBufferObject<T> m_VertexBufferObject;
};
