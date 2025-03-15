#include "Application.hh"

#include "BlockSpecification.hh"
#include "ThreadPool.hh"
#include "Utils/Logger.hh"

#include <glm/gtc/matrix_transform.hpp>

//
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

constexpr const int SCREEN_WIDTH = 1200;
constexpr const int SCREEN_HEIGHT = 800;

void Application::KeyCallback(int key, int action)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
            {
                int mode = glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
                glfwSetInputMode(m_Window, GLFW_CURSOR, mode);
                break;
            }

            case GLFW_KEY_R:
            {
                int mode;
                glGetIntegerv(GL_POLYGON_MODE, &mode);
                glPolygonMode(GL_FRONT_AND_BACK, mode == GL_FILL ? GL_LINE : GL_FILL);
                break;
            }

            case GLFW_KEY_M:
            {
                m_OpenImGui = !m_OpenImGui;
                break;
            }

            case GLFW_KEY_F:
            {
                auto position = m_Camera3D.GetPosition();
                position.z += 1e2;
                m_Camera3D.SetPosition(position.x, position.y, position.z);

                // m_SuperFast = !m_SuperFast;
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
    m_Camera3D.ProcessMouseScroll(static_cast<float>(yoffset));
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
}

void Application::InitializeImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    float xscale, yscale;
    glfwGetWindowContentScale(m_Window, &xscale, &yscale);

    float fontSize = 16.0f * xscale;  // Base size 16, scaled by DPI
    io.Fonts->AddFontFromFileTTF("resources/fonts/ComicSans.ttf", fontSize);

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

    m_Shader.Initialize("resources/shaders/vertex.glsl.vert", "resources/shaders/fragment.glsl.frag");
    m_Texture.Initialize("resources/textures/terrain.png");
    m_Camera3D = Camera3D(glm::vec3(kChunkWidth / 2.0, kChunkHeight / 2.0 + 10.0f, kChunkDepth / 2.0));

    m_World.SetRenderer(&m_Renderer);
    m_World.Initialize(glm::ivec2(m_Camera3D.GetPosition().x, m_Camera3D.GetPosition().z));

    glEnable(GL_BLEND);
    // glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // glCullFace(GL_BACK);
    // glFrontFace(GL_CW);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);

    glfwSwapInterval(1);
}

void Application::Destroy()
{
    GetThreadPool()->Destroy();

    m_Shader.Destroy();

    for (auto &&vertexArrayObject : m_Renderer.GetVertexArrayObjects())
    {
        vertexArrayObject.Destroy();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Application::Draw()
{
    // blue sky
    glClearColor(190 / 255.0f, 244 / 255.0f, 255 / 255.0f, 1.0f);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 proj = glm::perspective(glm::radians(m_Camera3D.GetZoom()), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = m_Camera3D.GetViewMatrix();
    static glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    glm::mat4 mvp = proj * view * model;

    m_Texture.Bind();
    m_Shader.Bind();
    m_Shader.SetUniform("u_Mvp", mvp);
    m_Shader.SetUniform("u_Texture", 0);

    for (const VertexArrayObject<VoxelVertex> &vertexArrayObject : m_Renderer.GetVertexArrayObjects())
    {
        vertexArrayObject.Bind();
        glDrawElements(GL_TRIANGLES, vertexArrayObject.GetIndicesCount(), GL_UNSIGNED_INT, 0);
    }

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
        Draw();
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
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_FPS, m_FPS);
    ImGui::Text("Chunk size: %zu", m_World.GetChunks().size());
    ImGui::InputInt("Render Distance", m_World.GetRenderDistancePointer());
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
