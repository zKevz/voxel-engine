#include "Shader.hh"

#include "Utils/Logger.hh"
#include "Utils/Utils.hh"

static GLuint CreateShader(std::string_view path, GLenum shaderType)
{
    std::string string = Utils::ReadFile(path);

    const char *data = string.data();

    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &data, nullptr);
    glCompileShader(shaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        LogError("ERROR::SHADER::COMPILATION_FAILED\n{}", infoLog);
        return 0;
    }

    return shaderID;
}

void Shader::Initialize(std::string_view vertexShaderPath, std::string_view fragmentShaderPath)
{
    GLuint vertexShaderID = CreateShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShaderID = CreateShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShaderID);
    glAttachShader(m_ID, fragmentShaderID);
    glLinkProgram(m_ID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    int success;
    char infoLog[512];
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        LogError("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}", infoLog);
        return;
    }
}

void Shader::Bind() const
{
    glUseProgram(m_ID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::Destroy() const
{
    glDeleteProgram(m_ID);
}

void Shader::SetUniform(std::string_view name, int value)
{
    glUniform1i(GetUniformID(name), value);
}

void Shader::SetUniform(std::string_view name, float value)
{
    glUniform1f(GetUniformID(name), value);
}

void Shader::SetUniform(std::string_view name, glm::vec2 value)
{
    glUniform2f(GetUniformID(name), value.x, value.y);
}

void Shader::SetUniform(std::string_view name, glm::vec3 value)
{
    glUniform3f(GetUniformID(name), value.x, value.y, value.z);
}

void Shader::SetUniform(std::string_view name, const glm::vec4 &value)
{
    glUniform4f(GetUniformID(name), value.x, value.y, value.z, value.w);
}

void Shader::SetUniform(std::string_view name, const glm::mat4 &matrix)
{
    glUniformMatrix4fv(GetUniformID(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformID(std::string_view name)
{
    if (m_UniformCache.contains(name))
    {
        return m_UniformCache[name];
    }

    int id = glGetUniformLocation(m_ID, name.data());
    if (id == -1)
    {
        LogError("No uniform variable with name: '{}'", name.data());
    }

    return m_UniformCache[name] = id;
}
