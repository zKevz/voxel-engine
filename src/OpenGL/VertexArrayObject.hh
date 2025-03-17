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
    void DestroyInner() const;

    void Set(VertexBufferObject<T> vertexBufferObject, std::vector<GLuint> indices);
    void SetVertexBufferObject(VertexBufferObject<T> vertexBufferObject);

    inline constexpr GLuint GetID() const
    {
        return m_ID;
    }

    inline constexpr GLuint GetIndicesCount() const
    {
        if (m_IndexBuffer.GetCount())
        {
            return m_IndexBuffer.GetCount();
        }

        return m_VertexBufferObject.GetCount();
    }

    inline constexpr IndexBuffer &GetIndexBuffer()
    {
        return m_IndexBuffer;
    }

    inline constexpr VertexBufferObject<T> &GetVertexBufferObject()
    {
        return m_VertexBufferObject;
    }

private:
    GLuint m_ID = 0;

    IndexBuffer m_IndexBuffer;
    VertexBufferObject<T> m_VertexBufferObject;
};
