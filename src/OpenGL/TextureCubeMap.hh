#pragma once
#include <glad/glad.h>

#include <string>

struct TextureCubeMapPath
{
    std::string RightPath;
    std::string LeftPath;
    std::string UpPath;
    std::string DownPath;
    std::string FrontPath;
    std::string BackPath;
};

class TextureCubeMap
{
public:
    void Initialize(TextureCubeMapPath path);
    void Bind(GLuint slot = 0) const;
    void Unbind() const;
    void Destroy() const;

    inline constexpr GLuint GetID() const
    {
        return m_ID;
    }

private:
    GLuint m_ID = 0;

    int m_Width = 0;
    int m_Height = 0;
    int m_Channels = 0;
};
