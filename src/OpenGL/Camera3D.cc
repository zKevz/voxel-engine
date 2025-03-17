#include "Camera3D.hh"

Camera3D::Camera3D(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
    m_Yaw = yaw;
    m_Zoom = kDefaultZoom;
    m_Pitch = pitch;
    m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_WorldUp = up;
    m_Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Position = position;
    m_MovementSpeed = kDefaultSpeed;
    m_MouseSensitivity = kDefaultSensitivity;

    UpdateCameraVectors();
}

Camera3D::Camera3D(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
    m_Yaw = yaw;
    m_Zoom = kDefaultZoom;
    m_Pitch = pitch;
    m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_WorldUp = glm::vec3(upX, upY, upZ);
    m_Position = glm::vec3(posX, posY, posZ);
    m_MovementSpeed = kDefaultSpeed;
    m_MouseSensitivity = kDefaultSensitivity;

    UpdateCameraVectors();
}

void Camera3D::Update(GLFWwindow *window, float deltaTime)
{
    m_Velocity = {};

    if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_NORMAL)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            m_Velocity.x = m_Front.x;
            m_Velocity.z = m_Front.z;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            m_Velocity.x = -m_Front.x;
            m_Velocity.z = -m_Front.z;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            m_Velocity.x = -m_Right.x;
            m_Velocity.z = -m_Right.z;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            m_Velocity.x = m_Right.x;
            m_Velocity.z = m_Right.z;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            m_Velocity.y = 1.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            m_Velocity.y = -1.0f;
        }

        if (m_SuperFast)
        {
            m_MovementSpeed = kDefaultSpeed * 10.0f;
        }
        else
        {
            m_MovementSpeed = kDefaultSpeed;
        }
    }

    if (m_EnablePhysics)
    {
        m_Velocity.y -= kGravity;
    }

    m_Position += m_Velocity * m_MovementSpeed * deltaTime;
}

void Camera3D::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    if (constrainPitch)
    {
        m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);
    }

    UpdateCameraVectors();
}

void Camera3D::ProcessMouseScroll(float yoffset)
{
    m_Zoom = glm::clamp(m_Zoom - yoffset, 1.0f, 45.0f);
}

void Camera3D::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
