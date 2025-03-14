#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct VertexBufferLayout
{
    GLuint Type;
    GLuint Count;
    GLbyte Normalized;

    inline constexpr GLuint GetSize() const
    {
        switch (Type)
        {
            case GL_FLOAT:
                return sizeof(GLfloat);

            case GL_UNSIGNED_INT:
                return sizeof(GLuint);

            case GL_UNSIGNED_BYTE:
                return sizeof(GLbyte);
        }

        assert(false);
        return 0;
    }
};

template<class T>
class VertexBufferObject
{
public:
    void Initialize();

    void AddByte(GLuint count);
    void AddFloat(GLuint count);
    void AddUInteger(GLuint count);

    void AddVertex(T vertex);
    void Build() const;

    void Bind() const;
    void Unbind() const;
    void Destroy() const;

private:
    GLuint m_ID = 0;

    std::vector<T> m_Vertex;
    std::vector<VertexBufferLayout> m_Layout;
};
