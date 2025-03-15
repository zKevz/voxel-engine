#pragma once
#include "OpenGL/Camera3D.hh"
#include "OpenGL/Shader.hh"
#include "OpenGL/Texture.hh"
#include "Renderer.hh"
#include "World.hh"

#include <GLFW/glfw3.h>

class Application
{
public:
    void Initialize();
    void Destroy();
    void Poll();
    void Draw();
    void Update(float deltaTime);

private:
    void InitializeGLFW();
    void InitializeImGui();
    void RenderImGui();
    void ProcessInput();
    void MouseCallback(double xposIn, double yposIn);
    void ProcessMouseScroll(double yoffset);
    void KeyCallback(int key, int action);

    static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);

    GLFWwindow *m_Window = nullptr;

    World m_World;
    Shader m_Shader;
    Texture m_Texture;
    Camera3D m_Camera3D;
    Renderer m_Renderer;

    bool m_FirstMouse = true;
    bool m_OpenImGui = true;
    bool m_SuperFast = false;

    float m_LastX = 0;
    float m_LastY = 0;
    float m_DeltaTime = 0;
    float m_LastFrame = 0;
    float m_FPS = 0;
    float m_FPSTimer = 0;
};

inline Application *GetApplication()
{
    static Application instance;
    return &instance;
}
