#pragma once
#include "Block.hh"

#include <array>

static constexpr int kChunkDepth = 32 * 5;
static constexpr int kChunkWidth = 32 * 5;
static constexpr int kChunkHeight = 32 * 5;

class Chunk
{
public:
    void Initialize();

    glm::vec3 Position;
    std::array<std::array<std::array<Block, kChunkDepth>, kChunkHeight>, kChunkWidth> Blocks;
};
