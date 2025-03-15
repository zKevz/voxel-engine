#pragma once

#include "Chunk.hh"

#include <future>
#include <map>
#include <queue>
#include <set>

static constexpr int kDefaultRenderDistance = 16;

using Chunks = std::map<std::pair<int, int>, Chunk>;

class Renderer;
class World
{
public:
    World();

    void AddChunk(glm::ivec2 position);
    void AddChunk(Chunk chunk);
    void Initialize(glm::ivec2 genesis = glm::ivec2(0, 0));
    void RemoveFuture(glm::ivec2 position);

    const Chunk *GetChunk(glm::ivec3 position);
    const Chunk *GetChunk(int x, int y, int z);

    BlockType GetBlockTypeAt(glm::ivec3 position);
    BlockType GetBlockTypeAt(int x, int y, int z);

    inline constexpr const std::map<std::pair<int, int>, Chunk> &GetChunks() const
    {
        return m_Chunks;
    }

    inline constexpr int GetRenderDistance() const
    {
        return m_RenderDistance;
    }

    inline constexpr int *GetRenderDistancePointer()
    {
        return &m_RenderDistance;
    }

    void SetRenderDistance(int renderDistance)
    {
        m_RenderDistance = renderDistance;
    }

    void SetRenderer(Renderer *renderer)
    {
        m_Renderer = renderer;
    }

private:
    void RemoveFarChunks(glm::ivec2 genesis);

    int m_Seed = rand();
    int m_RenderDistance = kDefaultRenderDistance;

    Renderer *m_Renderer = nullptr;

    Chunks m_Chunks;
    FastNoiseLite m_Noise;

    std::set<std::pair<int, int>> m_QueuedChunks;
    std::queue<std::pair<int, int>> m_RenderQueue;
    std::map<std::pair<int, int>, std::future<void>> m_Futures;
};
