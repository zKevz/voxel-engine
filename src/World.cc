#include "World.hh"

#include "BlockSpecification.hh"
#include "Renderer.hh"
#include "ThreadPool.hh"
#include "Utils/Logger.hh"
#include "Utils/Utils.hh"

World::World()
{
    m_Noise.SetSeed(1337);
    m_Noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_Noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_Noise.SetFrequency(0.002);
    m_Noise.SetFractalOctaves(8);
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

    GetThreadPool()->SubmitChunkAndRender(m_Renderer, m_Noise, chunkX, chunkZ);
    m_QueuedChunks.insert(key);
}

void World::AddChunk(Chunk &&chunk)
{
    auto key = std::make_pair(chunk.GetPosition().x, chunk.GetPosition().y);
    m_Chunks.insert({ key, std::move(chunk) });
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
    // removee all is out of range of render distance

    glm::ivec2 genesisPos;
    genesisPos.x = Utils::FloorDiv(genesis.x, kChunkWidth) * kChunkWidth;
    genesisPos.y = Utils::FloorDiv(genesis.y, kChunkDepth) * kChunkDepth;

    std::vector<std::pair<int, int>> toRemove;
    for (const auto &[pos, _] : m_Chunks)
    {
        glm::ivec2 chunkPos = glm::ivec2(pos.first, pos.second);
        glm::ivec2 distance = glm::ivec2(std::abs(chunkPos.x - genesisPos.x), std::abs(chunkPos.y - genesisPos.y));

        if (distance.x > m_RenderDistance * kChunkWidth || distance.y > m_RenderDistance * kChunkDepth)
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

void World::RemoveQueue(glm::ivec2 position)
{
    m_QueuedChunks.erase(std::make_pair(position.x, position.y));
}

void World::AddBlock(glm::ivec3 position, BlockType type)
{
    int chunkX = Utils::FloorDiv(position.x, kChunkWidth) * kChunkWidth;
    int chunkZ = Utils::FloorDiv(position.z, kChunkDepth) * kChunkDepth;

    auto key = std::make_pair(chunkX, chunkZ);
    Block &block = GetBlock(position);
    block.SetType(type);

    m_Renderer->ReRenderChunk(*this, m_Chunks[key], m_Noise);
}

void World::DestroyBlock(glm::ivec3 position)
{
    int chunkX = Utils::FloorDiv(position.x, kChunkWidth) * kChunkWidth;
    int chunkZ = Utils::FloorDiv(position.z, kChunkDepth) * kChunkDepth;

    auto key = std::make_pair(chunkX, chunkZ);
    Block &block = GetBlock(position);
    block.SetType(BlockType::Air);

    m_Renderer->ReRenderChunk(*this, m_Chunks[key], m_Noise);

    // is this special block in the edge!??!?!? also need to re-render chunk besides it
    if (position.x == chunkX)
    {
        key.first -= kChunkWidth;
        m_Renderer->ReRenderChunk(*this, m_Chunks[key], m_Noise);
        key.first += kChunkWidth;
    }
    else if (position.x == chunkX + kChunkWidth - 1)
    {
        key.first += kChunkWidth;
        m_Renderer->ReRenderChunk(*this, m_Chunks[key], m_Noise);
        key.first -= kChunkWidth;
    }

    if (position.z == chunkZ)
    {
        key.second -= kChunkDepth;
        m_Renderer->ReRenderChunk(*this, m_Chunks[key], m_Noise);
    }
    else if (position.z == chunkZ + kChunkDepth - 1)
    {
        key.second += kChunkDepth;
        m_Renderer->ReRenderChunk(*this, m_Chunks[key], m_Noise);
    }
}

Chunk *World::GetChunk(glm::ivec3 position)
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

Chunk *World::GetChunk(int x, int y, int z)
{
    return GetChunk(glm::ivec3(x, y, z));
}

Block &World::GetBlock(glm::ivec3 position)
{
    Chunk *chunk = GetChunk(position);
    assert(chunk);

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

    return chunk->GetBlock(localX, localY, localZ);
}

Block &World::GetBlock(int x, int y, int z)
{
    return GetBlock(glm::ivec3(x, y, z));
}

BlockType World::GetBlockType(glm::ivec3 position)
{
    Chunk *chunk = GetChunk(position);
    if (chunk == nullptr)
    {
        LogDebug("chunk at {} {} is nullptr", position.x, position.z);
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

    return chunk->GetBlock(localX, localY, localZ).GetType();
}

BlockType World::GetBlockType(int x, int y, int z)
{
    return GetBlockType(glm::ivec3(x, y, z));
}

bool World::IsBlockTransparent(glm::ivec3 position)
{
    return GetBlockSpecificationManager()->GetSpecification(GetBlockType(position)).IsTransparent();
}

bool World::IsBlockTransparent(int x, int y, int z)
{
    return IsBlockTransparent(glm::ivec3(x, y, z));
}
