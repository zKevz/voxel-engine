#include "TextureCubeMap.hh"

#include "Utils/Logger.hh"

#include <stb/stb_image.h>

static constexpr int kCubeMapSize = 6;

void TextureCubeMap::Initialize(TextureCubeMapPath path)
{
    // clang-format off
    std::array<std::string_view, kCubeMapSize> paths =
    {
        path.RightPath,
        path.LeftPath,
        path.UpPath,
        path.DownPath,
        path.FrontPath,
        path.BackPath,
    };
    // clang-format on

    // i love opengl consistency!!!! <3
    stbi_set_flip_vertically_on_load(0);

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    bool success = true;
    for (int i = 0; i < kCubeMapSize; ++i)
    {
        void *data = stbi_load(paths[i].data(), &m_Width, &m_Height, &m_Channels, 0);
        if (!data)
        {
            LogError("Failed to read image for cube map: {}", paths[i]);
            success = false;
            break;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    if (!success)
    {
        glDeleteTextures(1, &m_ID);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
}

void TextureCubeMap::Bind(GLuint slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

void TextureCubeMap::Unbind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubeMap::Destroy() const
{
    if (m_ID)
    {
        glDeleteTextures(1, &m_ID);
    }
}
