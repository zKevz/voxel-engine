#include "Camera3D.hh"

Camera3D::Camera3D(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
    m_Yaw = yaw;
    m_Zoom = kDefaultZoom;
    m_Pitch = pitch;
    m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_WorldUp = up;
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

void Camera3D::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = m_MovementSpeed * deltaTime;
    switch (direction)
    {
        case CameraMovement::Forward:
        {
            m_Position += m_Front * velocity;
            break;
        }

        case CameraMovement::Backward:
        {
            m_Position -= m_Front * velocity;
            break;
        }

        case CameraMovement::Left:
        {
            m_Position -= m_Right * velocity;
            break;
        }

        case CameraMovement::Right:
        {
            m_Position += m_Right * velocity;
            break;
        }

        case CameraMovement::Up:
        {
            m_Position.y += velocity * 2;
            break;
        }

        case CameraMovement::Down:
        {
            m_Position.y -= velocity;
            break;
        }
    }
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
