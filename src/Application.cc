#include "Application.hh"

#include "BlockSpecification.hh"
#include "ThreadPool.hh"
#include "Utils/Logger.hh"
#include "Utils/Utils.hh"

#include <glm/gtc/matrix_transform.hpp>

//
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

void Application::KeyCallback(int key, int action)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
            {
                int mode = glfwGetInputMode(m_Window, GLFW_CURSOR);
                m_FirstMouse = mode == GLFW_CURSOR_NORMAL;
                glfwSetInputMode(m_Window, GLFW_CURSOR, mode == GLFW_CURSOR_NORMAL ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
                break;
            }

            case GLFW_KEY_R:
            {
                m_RenderWireframe = !m_RenderWireframe;
                glPolygonMode(GL_FRONT_AND_BACK, m_RenderWireframe ? GL_LINE : GL_FILL);
                break;
            }

            case GLFW_KEY_M:
            {
                m_OpenImGui = !m_OpenImGui;
                break;
            }

            case GLFW_KEY_F:
            {
                m_SuperFast = !m_SuperFast;
                break;
            }
        }
    }
}

void Application::FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Application::MouseCallback(double xposIn, double yposIn)
{
    if (glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
    {
        return;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (m_FirstMouse)
    {
        m_LastX = xpos;
        m_LastY = ypos;
        m_FirstMouse = false;
    }

    float xoffset = xpos - m_LastX;
    float yoffset = m_LastY - ypos;

    m_LastX = xpos;
    m_LastY = ypos;
    m_Camera3D.ProcessMouseMovement(xoffset, yoffset);
}

void Application::ProcessMouseScroll(double yoffset)
{
    if (glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
    {
        return;
    }

    m_Camera3D.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Application::MouseButtonCallback(int button, int action)
{
    if (glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
    {
        return;
    }

    if (action == GLFW_PRESS)
    {
        switch (button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
            {
                RaycastBlockResult raycastBlockResult = Utils::RaycastBlock(m_Camera3D.GetPosition(), m_Camera3D.GetFront(), m_World);
                if (raycastBlockResult.Hit)
                {
                    m_World.DestroyBlock(raycastBlockResult.BlockPosition);
                }
                break;
            }

            case GLFW_MOUSE_BUTTON_RIGHT:
            {
                RaycastBlockResult raycastBlockResult = Utils::RaycastBlock(m_Camera3D.GetPosition(), m_Camera3D.GetFront(), m_World);
                if (raycastBlockResult.Hit)
                {
                    m_World.AddBlock(raycastBlockResult.BlockPosition + raycastBlockResult.FaceNormal, m_BlockTypeChosen);
                }
                break;
            }
        }
    }
}

void Application::InitializeGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Voxel", NULL, NULL);
    if (m_Window == NULL)
    {
        LogError("Failed to create GLFW window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        LogError("Failed to create GLAD");
        return;
    }

    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(m_Window, FrameBufferSizeCallback);
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xposIn, double yposIn) {
        GetApplication()->MouseCallback(xposIn, yposIn);
    });
    glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xoffset, double yoffset) {
        GetApplication()->ProcessMouseScroll(static_cast<float>(yoffset));
    });
    glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        GetApplication()->KeyCallback(key, action);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
        GetApplication()->MouseButtonCallback(button, action);
    });
}

void Application::InitializeImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    io.Fonts->AddFontFromFileTTF("resources/fonts/ComicSans.ttf", 16.0);

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Application::Initialize()
{
    InitializeGLFW();
    InitializeImGui();

    GetThreadPool()->Initialize();
    GetBlockSpecificationManager()->Initialize();

    m_Renderer.Initialize();
    m_Camera3D = Camera3D(glm::vec3(kChunkWidth / 2.0, kChunkHeight / 2.0 + 10.0f, kChunkDepth / 2.0));

    m_World.SetRenderer(&m_Renderer);
    m_World.Initialize(glm::ivec2(m_Camera3D.GetPosition().x, m_Camera3D.GetPosition().z));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1);
}

void Application::Destroy()
{
    GetThreadPool()->Destroy();

    m_Renderer.Destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Application::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Renderer.Render(m_World, m_Camera3D);

    RenderImGui();
}

void Application::Update(float deltaTime)
{
    float fpsUpdateInterval = 0.5f;

    m_DeltaTime = deltaTime - m_LastFrame;
    m_LastFrame = deltaTime;

    m_FPSTimer += m_DeltaTime;

    if (m_FPSTimer >= fpsUpdateInterval)
    {
        m_FPS = 1.0f / m_DeltaTime;
        m_FPSTimer = 0.0f;
    }

    m_World.Initialize(glm::ivec2(m_Camera3D.GetPosition().x, m_Camera3D.GetPosition().z));
    m_Renderer.PollQueue(m_World);
}

void Application::Poll()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        Update(glfwGetTime());
        ProcessInput();
        Render();
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void Application::RenderImGui()
{
    if (!m_OpenImGui)
    {
        return;
    }

    static float positions[3] = { 0.0f, 0.0f, 0.0f };

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Voxel");
    ImGui::Text("Press 'ESC' to toggle cursor");
    ImGui::Text("Press 'R' to see wireframe");
    ImGui::Text("Press 'M' to toggle imgui");
    ImGui::Text("Press 'F' to toggle super fast");
    ImGui::Text("Position: %.3f %.3f %.3f", m_Camera3D.GetPosition().x, m_Camera3D.GetPosition().y, m_Camera3D.GetPosition().z);

    static const char *skyTypes[] = { "blue", "brown", "gray", "yellow", "cool" };
    static int skyTypeIndex = 4;  // default cool because i like it the most

    if (ImGui::Combo("Sky Type", &skyTypeIndex, "blue\0brown\0gray\0yellow\0cool"))
    {
        if (skyTypeIndex == 4)
        {
            m_Renderer.InitializeSkyBoxTextureCool();
        }
        else
        {
            m_Renderer.InitializeSkyBoxTexture(skyTypes[skyTypeIndex]);
        }
    }

    // i love unsafe-ness
    if (ImGui::Combo("Block chosen", (int *) &m_BlockTypeChosen, BlockTypeToStringSeparatedByZeros()))
    {
        if (m_BlockTypeChosen == BlockType::Air)
        {
            m_BlockTypeChosen = BlockType::Grass;
        }
    }

    RaycastBlockResult raycastBlockResult = Utils::RaycastBlock(m_Camera3D.GetPosition(), m_Camera3D.GetFront(), m_World);
    ImGui::Text("Raycast: %d %d %d", raycastBlockResult.BlockPosition.x, raycastBlockResult.BlockPosition.y, raycastBlockResult.BlockPosition.z);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_FPS, m_FPS);
    ImGui::Text("Chunk size: %zu", m_World.GetChunks().size());
    ImGui::InputInt("Render Distance", m_World.GetRenderDistancePointer());
    ImGui::InputFloat("Ambient", m_Renderer.GetAmbientPointer());
    ImGui::InputFloat3("Light Source Direction", (float *) m_Renderer.GetLightSourceDirectionPointer());
    ImGui::Checkbox("Go very fast", &m_SuperFast);
    ImGui::InputFloat3("Teleport", positions);

    if (ImGui::Button("Teleport"))
    {
        m_Camera3D.SetPosition(positions[0], positions[1], positions[2]);
    }

    if (ImGui::Button("Forward 100x"))
    {
        auto position = m_Camera3D.GetPosition();
        position.z += 1e9;
        m_Camera3D.SetPosition(position.x, position.y, position.z);
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::ProcessInput()
{
    if (glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
    {
        return;
    }

    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Forward, m_DeltaTime, m_SuperFast);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Backward, m_DeltaTime, m_SuperFast);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Left, m_DeltaTime, m_SuperFast);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Right, m_DeltaTime, m_SuperFast);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Up, m_DeltaTime, m_SuperFast);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Down, m_DeltaTime, m_SuperFast);
    }
}
