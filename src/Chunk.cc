#include "Chunk.hh"

#include "BlockSpecification.hh"
#include "Utils/Logger.hh"

Chunk::Chunk()
{
    m_Blocks.resize(kChunkWidth);

    for (int i = 0; i < kChunkWidth; ++i)
    {
        m_Blocks[i].resize(kChunkHeight);

        for (int j = 0; j < kChunkHeight; ++j)
        {
            m_Blocks[i][j].resize(kChunkDepth);
        }
    }
}

inline float Normalize(float v, float a, float b)
{
    return (v - a) / (b - a);
}

static inline constexpr float Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

static inline std::vector<std::pair<float, float>> ContinentalnessCurve = {
    { -1.0f, -60.0f },  // deep ocean
    { -0.9f, -50.0f },  // deep ocean
    { -0.7f, -40.0f },  // deep ocean
    { -0.5f, -30.0f },  // warm ocean
    { -0.3f, -20.0f },  // plain
    { 0.0f, -5.0f },    // plain
    { 0.1f, 0.0f },     // plain
    { 0.3f, 40.0f },    // hills
    { 0.5f, 150.0f },   // mountain
    { 0.7f, 200.0f },   // mountain
    { 1.0, 250.0f },    // mountain
};

inline float MapContinentalness(float x)
{
    if (ContinentalnessCurve.empty())
    {
        return 0.0f;
    }

    if (x <= ContinentalnessCurve.front().first)
    {
        return ContinentalnessCurve.front().second;
    }

    if (x >= ContinentalnessCurve.back().first)
    {
        return ContinentalnessCurve.back().second;
    }

    for (int i = 0; i < ContinentalnessCurve.size() - 1; ++i)
    {
        if (x >= ContinentalnessCurve[i].first && x <= ContinentalnessCurve[i + 1].first)
        {
            float t = Normalize(x, ContinentalnessCurve[i].first, ContinentalnessCurve[i + 1].first);
            return Lerp(ContinentalnessCurve[i].second, ContinentalnessCurve[i + 1].second, t);
        }
    }

    return 0.0f;
}

static inline constexpr int kBaseHeight = 100;
static inline constexpr int kWaterHeightMax = 90;
static inline constexpr int kMountainHeightMin = 150;
static inline constexpr int kMountainSnowHeightMin = 200;

BlockType Chunk::GetBlockTypeAt(int x, int y, int z, const FastNoiseLite &noise)
{
    float continentalness = noise.GetNoise((float) x, (float) z);
    int height = kBaseHeight + MapContinentalness(continentalness);
    if (y < height)
    {
        if (y >= kMountainSnowHeightMin)
        {
            return BlockType::Snow;
        }

        if (y >= kMountainHeightMin)
        {
            return BlockType::Stone;
        }

        if (y == height - 1)
        {
            return BlockType::Grass;
        }
        else
        {
            return BlockType::Dirt;
        }
    }
    else
    {
        if (y <= kWaterHeightMax)
        {
            return BlockType::Water;
        }

        return BlockType::Air;
    }
}

void Chunk::Initialize(const FastNoiseLite &noise, glm::ivec2 position)
{
    auto now = std::chrono::high_resolution_clock::now();
    m_Position = position;

    for (int x = 0; x < kChunkWidth; ++x)
    {
        for (int z = 0; z < kChunkDepth; ++z)
        {
            for (int y = kChunkHeight - 1; y >= 0; --y)
            {
                Block &block = GetBlock(x, y, z);
                block.SetType(GetBlockTypeAt(position.x + x, y, position.y + z, noise));
                block.SetSkyLight(0);
                block.SetBlockLight(0);
            }
        }
    }

    InitializeSkyLight();
    InitializeBlockLight();

    auto end = std::chrono::high_resolution_clock::now();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - now).count();
    LogDebug("Took {} microseconds to generate a chunk", microseconds);
}

bool Chunk::IsTransparent(int x, int y, int z, const FastNoiseLite &noise)
{
    return GetBlockSpecificationManager()->GetSpecification(GetBlockTypeAt(x, y, z, noise)).IsTransparent();
}
