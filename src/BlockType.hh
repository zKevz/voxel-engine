#pragma once

#include <string>

// i love 255 blocks limit <3
enum class BlockType : unsigned char
{
    Air,
    Grass,
    Dirt,
    Stone,
    Bedrock,
    Sand,
    Snow,
    Water,
    Glass,
    OakLog,
    OakLeaves
};

static inline std::vector<const char *> kBlocksEnumName = { "Air", "Grass", "Dirt", "Stone", "Bedrock", "Sand", "Snow", "Water", "Glass" };

static inline const char *BlockTypeToStringSeparatedByZeros()
{
    static std::string result;
    if (result.empty())
    {
        for (const auto &block : kBlocksEnumName)
        {
            result.append(block);
            result.push_back('\0');
        }
    }

    return result.data();
}
