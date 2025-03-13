#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string_view>

class Shader
{
public:
    void Initialize(std::string_view vertexShaderPath, std::string_view fragmentShaderPath);

    void Bind() const;
    void Unbind() const;
    void Destroy() const;

    void SetUniform(std::string_view name, int value);
    void SetUniform(std::string_view name, float value);
    void SetUniform(std::string_view name, glm::vec2 value);
    void SetUniform(std::string_view name, glm::vec3 value);
    void SetUniform(std::string_view name, const glm::vec4 &value);
    void SetUniform(std::string_view name, const glm::mat4 &matrix);

    inline constexpr GLuint GetID() const { return m_ID; }

private:
    int GetUniformID(std::string_view name);

    GLuint m_ID = 0;

    std::unordered_map<std::string_view, int> m_UniformCache;
};
