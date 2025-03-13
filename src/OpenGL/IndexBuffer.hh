#pragma once
#include <glad/glad.h>

#include <initializer_list>

class IndexBuffer
{
public:
    void Initialize(std::initializer_list<GLuint> indices);

    void Bind() const;
    void Unbind() const;
    void Destroy() const;

    inline constexpr GLuint GetID() const { return m_ID; }
    inline constexpr GLuint GetCount() const { return m_Count; }

private:
    GLuint m_ID = 0;
    GLuint m_Count = 0;
};
