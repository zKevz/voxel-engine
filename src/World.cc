#include "World.hh"

void World::AddChunk()
{
    Chunk chunk;
    chunk.Initialize();

    m_Chunks.emplace_back(std::move(chunk));
}
