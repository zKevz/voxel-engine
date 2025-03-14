#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

enum class VertexDirection : uint32_t
{
    Front,
    Back,
    Top,
    Bottom,
    Left,
    Right,
    Count
};

enum class VertexUVType : uint32_t
{
    BottomLeft,
    BottomRight,
    TopRight,
    TopLeft
};

struct Vertex
{
    glm::vec3 Position;
    uint32_t Bits = 0;

    Vertex(glm::vec3 position, glm::uvec2 uv, VertexUVType uvType, VertexDirection direction) : Position(position)
    {
        Bits |= (uint32_t) (uvType);          // Bits 0-1
        Bits |= ((uint32_t) direction) << 2;  // Bits 2-4
        Bits |= uv.x << 5;                    // Bits 5-8
        Bits |= uv.y << 9;                    // Bits 9-12
    }

} __attribute__((packed));
