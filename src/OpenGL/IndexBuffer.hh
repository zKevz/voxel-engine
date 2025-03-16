#pragma once
#include <glad/glad.h>

#include <vector>

class IndexBuffer
{
public:
    void Set(std::vector<GLuint> indices);
    void Replace(std::vector<GLuint> indices);
    void Initialize(std::vector<GLuint> indices);

    void Bind() const;
    void Unbind() const;
    void Destroy() const;

    inline constexpr GLuint GetID() const
    {
        return m_ID;
    }

    inline constexpr GLuint GetCount() const
    {
        return m_Count;
    }

private:
    GLuint m_ID = 0;
    GLuint m_Count = 0;
};
