#include "Chunk.hh"

void Chunk::InitializeSkyLight()
{
    // for (int x = 0; x < kChunkWidth; ++x)
    // {
    //     for (int z = 0; z < kChunkDepth; ++z)
    //     {
    //         bool foundBlock = false;
    //         bool shouldMakeThisDark = false;

    //         for (int y = kChunkHeight - 1; y >= 0; --y)
    //         {
    //             Block &block = GetBlock(x, y, z);
    //             block.SetSkyLight(0);

    //             if (block.GetType() != BlockType::Air)
    //             {
    //                 if (shouldMakeThisDark)
    //                 {
    //                     block.SetBlockLight(kMaximumLight - 5);
    //                     break;
    //                 }
    //                 else
    //                 {
    //                     foundBlock = true;
    //                     block.SetBlockLight(kMaximumLight);
    //                 }
    //             }
    //             else if (foundBlock)
    //             {
    //                 shouldMakeThisDark = true;
    //             }
    //         }
    //     }
    // }
}

void Chunk::InitializeBlockLight()
{
}
