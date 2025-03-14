#include "Chunk.hh"

#include "Utils/Logger.hh"

#include <FastNoiseLite/FastNoiseLite.h>

void Chunk::Initialize()
{
    // Setup terrain noise generator with reduced frequency for gentler slopes
    FastNoiseLite terrainNoise;
    terrainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    terrainNoise.SetSeed(rand());
    terrainNoise.SetFrequency(0.005f);  // Reduced from 0.01f for smoother terrain

    for (int x = 0; x < kChunkWidth; x++)
    {
        for (int z = 0; z < kChunkDepth; z++)
        {
            // Sample noise and scale height
            float noiseVal = terrainNoise.GetNoise((float) (x), (float) (z));
            int height = static_cast<int>((noiseVal + 1.0f) * 0.5f * (80 - 1));  // Map to [0, CHUNK_SIZE_Y]

            for (int y = 0; y < kChunkHeight; y++)
            {
                if (y <= height)
                {
                    Blocks[x][y][z].Type = BlockType::Grass;
                }
            }
        }
    }

    return;

    // Secondary noise with much lower impact
    FastNoiseLite detailNoise;
    detailNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    detailNoise.SetSeed(rand() + 1);
    detailNoise.SetFrequency(0.03f);  // Reduced from 0.05f

    // Calculate global position of this chunk
    int globalOffsetX = kChunkWidth;
    int globalOffsetZ = kChunkDepth;

    // World generation constants
    const int baseLevel = kChunkHeight / 2 - 5;  // Base terrain level
    const int heightVariation = 15;              // Reduced height variation from 20 to 5

    for (int x = 0; x < kChunkWidth; ++x)
    {
        for (int z = 0; z < kChunkDepth; ++z)
        {
            // Calculate world position
            float worldX = (float) (x + globalOffsetX);
            float worldZ = (float) (z + globalOffsetZ);

            // Generate base terrain height with reduced impact
            float heightValue = terrainNoise.GetNoise(worldX, worldZ);
            heightValue = (heightValue + 1.0f) * 0.5f;  // Convert from -1:1 to 0:1

            // Add minimal variation with secondary noise
            float detailValue = detailNoise.GetNoise(worldX, worldZ) * 0.1f;  // Reduced from 0.2f
            heightValue += detailValue;
            heightValue = std::max(0.0f, std::min(1.0f, heightValue));  // Clamp between 0 and 1

            // Scale height with much smaller multiplier for flatter terrain
            int terrainHeight = baseLevel + (int) (heightValue * heightVariation);

            // Ensure terrain height is within boundaries
            terrainHeight = std::min(terrainHeight, kChunkHeight - 4);
            terrainHeight = std::max(terrainHeight, 5);

            // Fill blocks from bottom to terrain height
            for (int y = 0; y < kChunkHeight; ++y)
            {
                // Default to air
                Blocks[x][y][z].Type = BlockType::Air;

                if (y == 0)
                {
                    // Bedrock at the very bottom
                    Blocks[x][y][z].Type = BlockType::Bedrock;
                }
                else if (y < terrainHeight - 3)
                {
                    // Stone layer
                    Blocks[x][y][z].Type = BlockType::Stone;
                }
                else if (y < terrainHeight)
                {
                    // Dirt layer
                    Blocks[x][y][z].Type = BlockType::Dirt;
                }
                else if (y == terrainHeight)
                {
                    // Top block is grass if above water level, dirt if underwater
                    Blocks[x][y][z].Type = (y > baseLevel) ? BlockType::Grass : BlockType::Dirt;
                }
                else if (y <= baseLevel)
                {
                    // Water up to base level
                    Blocks[x][y][z].Type = BlockType::Water;
                }
            }
        }
    }

    // Reduce stone patches significantly
    for (int x = 0; x < kChunkWidth; ++x)
    {
        for (int z = 0; z < kChunkDepth; ++z)
        {
            float worldX = (float) (x + globalOffsetX);
            float worldZ = (float) (z + globalOffsetZ);

            for (int y = 1; y <= baseLevel + 2; ++y)
            {
                if (Blocks[x][y][z].Type == BlockType::Dirt || Blocks[x][y][z].Type == BlockType::Grass)
                {
                    // Much stricter threshold for stone outcroppings
                    float stoneNoise = terrainNoise.GetNoise(worldX * 3.0f, (float) y * 0.5f, worldZ * 3.0f);
                    if (stoneNoise > 0.85f && y <= baseLevel + 1)  // Increased threshold from 0.7f to 0.85f
                    {
                        Blocks[x][y][z].Type = BlockType::Stone;
                    }
                }
            }
        }
    }
}
