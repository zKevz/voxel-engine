#include "Application.hh"

#include "Utils/Logger.hh"

#include <glm/gtc/matrix_transform.hpp>

constexpr const int SCREEN_WIDTH = 800;
constexpr const int SCREEN_HEIGHT = 600;

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void Application::FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Application::MouseCallback(double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    m_Camera3D.ProcessMouseMovement(xoffset, yoffset);
}

void Application::ProcessMouseScroll(double yoffset)
{
    m_Camera3D.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Application::Initialize()
{
    LogInfo("Hi!");

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

    glfwSetFramebufferSizeCallback(m_Window, FrameBufferSizeCallback);
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xposIn, double yposIn) {
        GetApplication()->MouseCallback(xposIn, yposIn);
    });
    glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xoffset, double yoffset) {
        GetApplication()->ProcessMouseScroll(static_cast<float>(yoffset));
    });

    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    m_Shader.Initialize("shaders/vertex.glsl.vert", "shaders/fragment.glsl.frag");
    m_Camera3D = Camera3D(glm::vec3(0.0f, 0.0f, 3.0f));

    m_VertexArrayObject.Initialize();
    m_VertexArrayObject.Bind();

    // clang-format off
    m_IndexBuffer.Initialize({
        0, 2, 1, 0, 3, 2,
        3, 7, 2, 7, 6, 2,
        1, 2, 6, 1, 6, 5,
        4, 7, 3, 4, 3, 0,
        0, 4, 5, 0, 5, 1,
        5, 6, 7, 5, 7, 4
    });
    // clang-format on

    m_VertexBufferObject.Initialize();
    m_VertexBufferObject.AddFloat(3);
    m_VertexBufferObject.AddVertices(Vertex { .Position = glm::vec3(-0.5f, -0.5f, 0.0f) });  // bottom left front
    m_VertexBufferObject.AddVertices(Vertex { .Position = glm::vec3(0.5f, -0.5f, 0.0f) });   // bottom right front
    m_VertexBufferObject.AddVertices(Vertex { .Position = glm::vec3(0.5f, 0.5f, 0.0f) });    // top right front
    m_VertexBufferObject.AddVertices(Vertex { .Position = glm::vec3(-0.5f, 0.5f, 0.0f) });   // top left front

    m_VertexBufferObject.AddVertices(Vertex { .Position = glm::vec3(-0.5f, -0.5f, 1.0f) });  // bottom left back
    m_VertexBufferObject.AddVertices(Vertex { .Position = glm::vec3(0.5f, -0.5f, 1.0f) });   // bottom right back
    m_VertexBufferObject.AddVertices(Vertex { .Position = glm::vec3(0.5f, 0.5f, 1.0f) });    // top right back
    m_VertexBufferObject.AddVertices(Vertex { .Position = glm::vec3(-0.5f, 0.5f, 1.0f) });   // top left back
    m_VertexBufferObject.Build();

    m_VertexArrayObject.Unbind();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        fps = 1.0f / deltaTime;
        fpsTimer += deltaTime;

        if (fpsTimer >= fpsUpdateInterval)
        {
            std::string title = fmt::format("Your Application - FPS: {}", fps);
            glfwSetWindowTitle(m_Window, title.data());

            fpsTimer = 0.0f;
        }

        ProcessInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 proj =
            glm::perspective(glm::radians(m_Camera3D.Zoom), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f);

        glm::mat4 view = m_Camera3D.GetViewMatrix();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        glm::mat4 mvp = proj * view * model;

        m_Shader.Bind();
        m_Shader.SetUniform("u_Mvp", mvp);
        m_Shader.SetUniform("u_Color", glm::vec4(1.0, 1.0, 0.0, 1.0));
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
        m_Camera3D.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(LEFT, deltaTime);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_Camera3D.ProcessKeyboard(RIGHT, deltaTime);
    }
}
