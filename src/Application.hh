#pragma once
#include "OpenGL/Camera3D.hh"
#include "Renderer.hh"
#include "World.hh"

#include <GLFW/glfw3.h>

class Application
{
public:
    void Initialize();
    void Destroy();
    void Poll();
    void Render();
    void Update(float time);

private:
    void InitializeGLFW();
    void InitializeImGui();
    void RenderImGui();
    void MouseCallback(double xposIn, double yposIn);
    void MouseButtonCallback(int button, int action);
    void ProcessMouseScroll(double yoffset);
    void KeyCallback(int key, int action);

    static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);

    GLFWwindow *m_Window = nullptr;

    World m_World;
    BlockType m_BlockTypeChosen = BlockType::Grass;

    Camera3D m_Camera3D;
    Renderer m_Renderer;

    int m_RenderDistance = kDefaultRenderDistance;

    bool m_OpenImGui = true;
    bool m_FirstMouse = true;
    bool m_RenderWireframe = false;

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
