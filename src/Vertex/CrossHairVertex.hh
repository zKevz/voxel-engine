#pragma once
#include <glm/glm.hpp>

struct CrossHairVertex
{
    glm::vec2 Position;

    CrossHairVertex(glm::vec2 position) : Position(position)
    {
    }
} __attribute__((packed));
