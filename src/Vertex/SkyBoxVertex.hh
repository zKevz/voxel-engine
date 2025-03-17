#pragma once
#include <glm/glm.hpp>

struct SkyBoxVertex
{
    glm::vec3 Position;

    SkyBoxVertex(glm::vec3 position) : Position(position)
    {
    }
} __attribute__((packed));
