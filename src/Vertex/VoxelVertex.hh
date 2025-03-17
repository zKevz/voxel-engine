#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

enum class VoxelVertexDirection : uint32_t
{
    Front,
    Back,
    Top,
    Bottom,
    Left,
    Right,
    Count
};

enum class VoxelVertexUVType : uint32_t
{
    BottomLeft,
    BottomRight,
    TopRight,
    TopLeft
};

struct VoxelVertex
{
    glm::vec3 Position;
    uint32_t Bits = 0;
    glm::vec3 BlockPosition;

    VoxelVertex(glm::vec3 position, glm::uvec2 uv, VoxelVertexUVType uvType, VoxelVertexDirection direction, glm::vec3 blockPosition, uint8_t skyLight, uint8_t blockLight) : Position(position)
    {
        Bits |= (uint32_t) (uvType);          // Bits 0-1
        Bits |= ((uint32_t) direction) << 2;  // Bits 2-4
        Bits |= uv.x << 5;                    // Bits 5-8
        Bits |= uv.y << 9;                    // Bits 9-12
        Bits |= skyLight << 13;               // Bits 13-16 (4 bits for sky light)
        Bits |= blockLight << 17;             // Bits 17-20 (4 bits for block light)
        BlockPosition = blockPosition;
    }

} __attribute__((packed));
