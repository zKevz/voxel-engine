#include "Application.hh"

#include "BlockSpecification.hh"
#include "Utils/Logger.hh"

#include <glm/gtc/matrix_transform.hpp>

constexpr const int SCREEN_WIDTH = 800;
constexpr const int SCREEN_HEIGHT = 600;

void Application::FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Application::MouseCallback(double xposIn, double yposIn)
{
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
}

void Application::RenderBlock(const Block &block, glm::vec3 position)
{
    float pf = 0.5f;
    float start = 0.5f;

    const BlockSpecification &specification = GetBlockSpecificationManager()->GetSpecification(block.Type);

    // front face
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Front), VertexUVType::BottomLeft, VertexDirection::Front));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Front), VertexUVType::BottomRight, VertexDirection::Front));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Front), VertexUVType::TopRight, VertexDirection::Front));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Front), VertexUVType::TopLeft, VertexDirection::Front));

    // Right face
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Right), VertexUVType::BottomLeft, VertexDirection::Right));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Right), VertexUVType::BottomRight, VertexDirection::Right));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Right), VertexUVType::TopRight, VertexDirection::Right));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Right), VertexUVType::TopLeft, VertexDirection::Right));

    // Back face
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Back), VertexUVType::BottomLeft, VertexDirection::Back));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Back), VertexUVType::BottomRight, VertexDirection::Back));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Back), VertexUVType::TopRight, VertexDirection::Back));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Back), VertexUVType::TopLeft, VertexDirection::Back));

    // Left face
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Left), VertexUVType::BottomLeft, VertexDirection::Left));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Left), VertexUVType::BottomRight, VertexDirection::Left));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Left), VertexUVType::TopRight, VertexDirection::Left));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Left), VertexUVType::TopLeft, VertexDirection::Left));

    // Top face
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Top), VertexUVType::BottomLeft, VertexDirection::Top));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Top), VertexUVType::BottomRight, VertexDirection::Top));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Top), VertexUVType::TopRight, VertexDirection::Top));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Top), VertexUVType::TopLeft, VertexDirection::Top));

    // Bottom face
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Bottom), VertexUVType::BottomLeft, VertexDirection::Bottom));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VertexDirection::Bottom), VertexUVType::BottomRight, VertexDirection::Bottom));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start + pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Bottom), VertexUVType::TopRight, VertexDirection::Bottom));
    m_VertexBufferObject.AddVertex(Vertex(glm::vec3(start - pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VertexDirection::Bottom), VertexUVType::TopLeft, VertexDirection::Bottom));
}

void Application::Initialize()
{
    InitializeGLFW();
    GetBlockSpecificationManager()->Initialize();

    m_Shader.Initialize("resources/shaders/vertex.glsl.vert", "resources/shaders/fragment.glsl.frag");
    m_Texture.Initialize("resources/textures/terrain.png");
    m_Camera3D = Camera3D(glm::vec3(0.0f, 0.0f, 50.0f));
    m_World.AddChunk();

    m_VertexArrayObject.Initialize();
    m_VertexArrayObject.Bind();

    m_VertexBufferObject.Initialize();
    m_VertexBufferObject.AddFloat(3);
    m_VertexBufferObject.AddUInteger(1);

    std::vector<GLuint> indices;
    GLuint index = 0;
    for (const Chunk &chunk : m_World.GetChunks())
    {
        for (int x = 0; x < kChunkWidth; ++x)
        {
            for (int y = 0; y < kChunkHeight; ++y)
            {
                for (int z = 0; z < kChunkDepth; ++z)
                {
                    if (chunk.Blocks[x][y][z].Type != BlockType::Air)
                    {
                        RenderBlock(chunk.Blocks[x][y][z], glm::vec3((float) x, (float) y, (float) z));

                        for (int i = 0; i < 6; ++i)
                        {
                            indices.push_back(index);
                            indices.push_back(index + 2);
                            indices.push_back(index + 1);
                            indices.push_back(index);
                            indices.push_back(index + 3);
                            indices.push_back(index + 2);
                            index += 4;
                        }
                    }
                }
            }
        }
    }

    m_IndexBuffer.Initialize(indices);
    m_VertexBufferObject.Build();
    m_VertexArrayObject.Unbind();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSwapInterval(1);
}

void Application::Destroy()
{
    m_Shader.Destroy();
    m_IndexBuffer.Destroy();
    m_VertexArrayObject.Destroy();
    m_VertexBufferObject.Destroy();

    glfwTerminate();
}

void Application::Poll()
{
    float fps = 0.0f;
    float fpsUpdateInterval = 0.5f;
    float fpsTimer = 0.0f;

    while (!glfwWindowShouldClose(m_Window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        m_DeltaTime = currentFrame - m_LastFrame;
        m_LastFrame = currentFrame;

        fps = 1.0f / m_DeltaTime;
        fpsTimer += m_DeltaTime;

        if (fpsTimer >= fpsUpdateInterval)
        {
            std::string title = fmt::format("Your Application - FPS: {}", fps);
            glfwSetWindowTitle(m_Window, title.data());

            fpsTimer = 0.0f;
        }

        ProcessInput();

        glClearColor(190 / 255.0f, 244 / 255.0f, 255 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = glm::perspective(glm::radians(m_Camera3D.GetZoom()), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f);

        glm::mat4 view = m_Camera3D.GetViewMatrix();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        glm::mat4 mvp = proj * view * model;

        m_Shader.Bind();
        m_Shader.SetUniform("u_Mvp", mvp);
        // m_Shader.SetUniform("u_Color", glm::vec4(1.0, 1.0, 0.0, 1.0));
        m_Texture.Bind();
        m_Shader.SetUniform("u_Texture", 0);
        m_VertexArrayObject.Bind();

        glDrawElements(GL_TRIANGLES, m_IndexBuffer.GetCount(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void Application::ProcessInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_Window, true);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Forward, m_DeltaTime);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Backward, m_DeltaTime);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Left, m_DeltaTime);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Right, m_DeltaTime);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Up, m_DeltaTime);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(CameraMovement::Down, m_DeltaTime);
    }
}
