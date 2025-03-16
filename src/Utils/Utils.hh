#pragma once
#include "World.hh"

#include <glm/glm.hpp>

#include <string>
#include <string_view>

struct RaycastBlockResult
{
    bool Hit = false;
    glm::ivec3 FaceNormal;
    glm::ivec3 BlockPosition;
};

namespace Utils
{
    std::string ReadFile(std::string_view path);
    RaycastBlockResult RaycastBlock(glm::vec3 origin, glm::vec3 direction, World &world, float maxDistance = 10.0f, float stepSize = 0.1f);

    inline constexpr int FloorDiv(int x, int divisor)
    {
        return (x >= 0) ? (x / divisor) : ((x + 1) / divisor - 1);
    }

}  // namespace Utils
