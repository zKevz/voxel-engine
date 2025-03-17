#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement
{
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};

static constexpr float kDefaultYaw = -90.0f;
static constexpr float kDefaultPitch = 0.0f;
static constexpr float kDefaultSpeed = 10.f;
static constexpr float kDefaultSensitivity = 0.1f;
static constexpr float kDefaultZoom = 45.0f;
static constexpr float kGravity = 9.8f;

class World;
class Camera3D
{
public:
    Camera3D(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = kDefaultYaw, float pitch = kDefaultPitch);
    Camera3D(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    void ProcessKeyboard(CameraMovement direction, float deltaTime, bool superFast);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    inline constexpr float GetZoom() const
    {
        return m_Zoom;
    }

    inline constexpr glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }

    inline constexpr glm::vec3 GetPosition() const
    {
        return m_Position;
    }

    inline constexpr glm::vec3 GetFront() const
    {
        return m_Front;
    }

    void SetPosition(float x, float y, float z)
    {
        m_Position = glm::vec3(x, y, z);
    }

    void SetEnablePhysics(bool enable)
    {
        m_EnablePhysics = enable;
    }

private:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;
    glm::vec3 m_Velocity;

    float m_Yaw;
    float m_Zoom;
    float m_Pitch;
    float m_MovementSpeed;
    float m_MouseSensitivity;

    bool m_InGround = false;
    bool m_EnablePhysics = true;

    void UpdateCameraVectors();
};
