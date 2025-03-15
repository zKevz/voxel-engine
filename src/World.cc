#include "World.hh"

#include "Renderer.hh"
#include "ThreadPool.hh"
#include "Utils/Logger.hh"
#include "Utils/Utils.hh"

World::World()
{
    m_Noise.SetSeed(m_Seed);
    m_Noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_Noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_Noise.SetFrequency(0.1f);
    m_Noise.SetFractalLacunarity(2.0f);
    m_Noise.SetFractalGain(0.5f);
}

static void AddChunkAndRender(Renderer *renderer, FastNoiseLite noise, int chunkX, int chunkZ)
{
    Chunk chunk;
    chunk.Initialize(noise, glm::ivec2(chunkX, chunkZ));

    LogDebug("Adding chunk: ({},{})", chunkX, chunkZ);

    renderer->RenderChunk(noise, std::move(chunk));
}

void World::AddChunk(glm::ivec2 position)
{
    int chunkX = Utils::FloorDiv(position.x, kChunkWidth) * kChunkWidth;
    int chunkZ = Utils::FloorDiv(position.y, kChunkDepth) * kChunkDepth;

    auto key = std::make_pair(chunkX, chunkZ);
    if (m_Chunks.contains(key) || m_QueuedChunks.contains(key))
    {
        return;
    }

    auto noise = m_Noise;
    auto renderer = m_Renderer;

    GetThreadPool()->SubmitChunkAndRender(m_Renderer, m_Noise, chunkX, chunkZ);

    m_QueuedChunks.insert(key);
    // auto future = std::async(AddChunkAndRender, m_Renderer, m_Noise, chunkX, chunkZ);
    // m_Futures.insert_or_assign(key, std::move(future));
}

void World::AddChunk(Chunk chunk)
{
    auto key = std::make_pair(chunk.GetPosition().x, chunk.GetPosition().y);
    m_Chunks.insert({ key, chunk });
}

void World::Initialize(glm::ivec2 genesis)
{
    AddChunk(genesis);

    for (int currentRenderDistance = 1; currentRenderDistance <= m_RenderDistance; ++currentRenderDistance)
    {
        for (int x = -currentRenderDistance; x <= currentRenderDistance; ++x)
        {
            AddChunk(genesis + glm::ivec2(x * kChunkWidth, currentRenderDistance * kChunkDepth));
            AddChunk(genesis + glm::ivec2(x * kChunkWidth, (-currentRenderDistance) * kChunkDepth));

            if (x != -currentRenderDistance && x != currentRenderDistance)
            {
                AddChunk(genesis + glm::ivec2((-currentRenderDistance) * kChunkWidth, x * kChunkDepth));
                AddChunk(genesis + glm::ivec2(currentRenderDistance * kChunkWidth, x * kChunkDepth));
            }
        }
    }

    RemoveFarChunks(genesis);
}

void World::RemoveFarChunks(glm::ivec2 genesis)
{
    glm::ivec2 genesisPos;
    genesisPos.x = Utils::FloorDiv(genesis.x, kChunkWidth) * kChunkWidth;
    genesisPos.y = Utils::FloorDiv(genesis.y, kChunkDepth) * kChunkDepth;

    // remove all is out of range of render distance
    std::vector<std::pair<int, int>> toRemove;
    for (const auto &[pos, _] : m_Chunks)
    {
        glm::ivec2 chunkPos = glm::ivec2(pos.first, pos.second);
        glm::ivec2 distance = glm::ivec2(std::abs(chunkPos.x - genesisPos.x), std::abs(chunkPos.y - genesisPos.y));

        int renderDistance = (m_RenderDistance) *kChunkWidth;
        if (distance.x > renderDistance || distance.y > renderDistance)
        {
            toRemove.push_back(pos);
        }
    }

    for (auto &&pair : toRemove)
    {
        LogDebug("Removing chunk {} {}", pair.first, pair.second);
        m_Renderer->DestroyChunk(m_Chunks[pair].GetVertexArrayObjectID());

        m_Chunks.erase(pair);
    }
}

void World::RemoveFuture(glm::ivec2 position)
{
    auto key = std::make_pair(position.x, position.y);
    m_Futures.erase(key);
    m_QueuedChunks.erase(key);
}

const Chunk *World::GetChunk(glm::ivec3 position)
{
    int chunkX = Utils::FloorDiv(position.x, kChunkWidth) * kChunkWidth;
    int chunkZ = Utils::FloorDiv(position.z, kChunkDepth) * kChunkDepth;

    auto key = std::make_pair(chunkX, chunkZ);
    auto it = m_Chunks.find(key);
    if (it != m_Chunks.end())
    {
        return &it->second;
    }

    return nullptr;
}

const Chunk *World::GetChunk(int x, int y, int z)
{
    return GetChunk(glm::ivec3(x, y, z));
}

BlockType World::GetBlockTypeAt(glm::ivec3 position)
{
    const Chunk *chunk = GetChunk(position);
    if (chunk == nullptr)
    {
        return Chunk::GetBlockTypeAt(position.x, position.y, position.z, m_Noise);
    }

    int localX = position.x % kChunkWidth;
    int localY = position.y;
    int localZ = position.z % kChunkDepth;

    if (localX < 0)
    {
        localX += kChunkWidth;
    }

    if (localZ < 0)
    {
        localZ += kChunkDepth;
    }

    if (localX < 0 || localY < 0 || localZ < 0 || localX >= kChunkWidth || localY >= kChunkHeight || localZ >= kChunkDepth)
    {
        LogInfo("what..");
        return BlockType::Air;
    }

    return chunk->GetBlockType(localX, localY, localZ);
}

BlockType World::GetBlockTypeAt(int x, int y, int z)
{
    return GetBlockTypeAt(glm::ivec3(x, y, z));
}
