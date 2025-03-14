#pragma once
#include <glad/glad.h>

#include <string_view>

class Texture
{
public:
    void Initialize(std::string_view path);
    void Bind(GLuint slot = 0) const;
    void Unbind() const;
    void Destroy() const;

    inline constexpr GLuint GetID() const { return m_ID; }

private:
    GLuint m_ID = 0;

    int m_Width = 0;
    int m_Height = 0;
    int m_Channels = 0;
};
