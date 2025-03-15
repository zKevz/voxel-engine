#include "Chunk.hh"

static constexpr float kBaseSurfaceY = 80.0f;
static constexpr float kMountainHeightY = 200.0f;  // Taller mountains
static constexpr float kHillHeightY = 2.0f;        // Small hills

static constexpr float kMountainFrequency = 0.1f;  // Higher frequency = more change
static constexpr float kHillFrequency = 0.7f;      // More detail for hills

BlockType Chunk::GetBlockTypeAt(int x, int y, int z, const FastNoiseLite &noise)
{
    // Hill base noise
    float hillNoise = noise.GetNoise((float) x * kHillFrequency, (float) z * kHillFrequency) * kHillHeightY;

    // Mountain noise
    float mountainNoiseRaw = noise.GetNoise((float) x * kMountainFrequency, (float) z * kMountainFrequency);
    float mountainNoise = std::max(0.0f, mountainNoiseRaw) * kMountainHeightY;

    // Combined height
    float surfaceY = kBaseSurfaceY + hillNoise + mountainNoise;

    // Block type selection
    if (y < surfaceY)
    {
        // Check elevation for biomes
        if (surfaceY > 120.0f)  // high mountain -> stone only
        {
            return BlockType::Stone;
        }
        else if (surfaceY > 100.0f)  // medium height -> dirt and stone
        {
            return BlockType::Stone;
        }
        else  // normal ground level -> grass, dirt, stone
        {
            if (y > surfaceY - 3)
                return BlockType::Grass;
            else if (y > surfaceY - 6)
                return BlockType::Dirt;
            else
                return BlockType::Stone;
        }
    }

    return BlockType::Air;
}

void Chunk::Initialize(const FastNoiseLite &noise, glm::ivec2 position)
{
    m_Position = position;

    for (int x = 0; x < kChunkWidth; ++x)
    {
        for (int y = 0; y < kChunkHeight; ++y)
        {
            for (int z = 0; z < kChunkDepth; ++z)
            {
                m_Blocks[x][y][z].Type = GetBlockTypeAt(position.x + x, y, position.y + z, noise);
            }
        }
    }
}
