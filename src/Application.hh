#pragma once
#include "OpenGL/Camera3D.hh"
#include "OpenGL/Shader.hh"
#include "OpenGL/Texture.hh"
#include "OpenGL/Vertex.hh"
#include "OpenGL/VertexArrayObject.hh"
#include "OpenGL/VertexBufferObject.hh"
#include "World.hh"

#include <GLFW/glfw3.h>

class Application
{
public:
    void Initialize();
    void Destroy();
    void Poll();

private:
    void InitializeGLFW();
    void ProcessInput();
    void MouseCallback(double xposIn, double yposIn);
    void ProcessMouseScroll(double yoffset);
    void RenderBlock(const Block &block, glm::vec3 position);
    static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);

    GLFWwindow *m_Window = nullptr;

    World m_World;
    Shader m_Shader;
    Texture m_Texture;
    Camera3D m_Camera3D;
    IndexBuffer m_IndexBuffer;
    VertexArrayObject m_VertexArrayObject;
    VertexBufferObject<Vertex> m_VertexBufferObject;

    bool m_FirstMouse = true;
    float m_LastX = 0;
    float m_LastY = 0;
    float m_DeltaTime = 0;
    float m_LastFrame = 0;
};

inline Application *GetApplication()
{
    static Application instance;
    return &instance;
}
