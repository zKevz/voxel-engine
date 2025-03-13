#pragma once
#include "OpenGL/Camera3D.hh"
#include "OpenGL/Shader.hh"
#include "OpenGL/Vertex.hh"
#include "OpenGL/VertexArrayObject.hh"
#include "OpenGL/VertexBufferObject.hh"

#include <GLFW/glfw3.h>

class Application
{
public:
    void Initialize();
    void Destroy();
    void Poll();

private:
    void ProcessInput();
    void MouseCallback(double xposIn, double yposIn);
    void ProcessMouseScroll(double yoffset);
    static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);

    GLFWwindow *m_Window = nullptr;

    Shader m_Shader;
    Camera3D m_Camera3D;
    IndexBuffer m_IndexBuffer;
    VertexArrayObject m_VertexArrayObject;
    VertexBufferObject<Vertex> m_VertexBufferObject;
};

inline Application *GetApplication()
{
    static Application instance;
    return &instance;
}
