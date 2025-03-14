#pragma once

#include "Chunk.hh"

#include <vector>

class World
{
public:
    void AddChunk();

    inline constexpr const std::vector<Chunk> &GetChunks() const
    {
        return m_Chunks;
    }

private:
    std::vector<Chunk> m_Chunks;
};
