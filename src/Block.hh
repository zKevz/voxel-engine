#pragma once

#include "BlockType.hh"

#include <glm/glm.hpp>

constexpr int kMaximumLight = (1 << 4) - 1;

class Block
{
public:
    bool IsTransparent() const;

    inline constexpr uint8_t GetSkyLight() const
    {
        return m_SkyLight;
    }

    inline constexpr uint8_t GetBlockLight() const
    {
        return m_BlockLight;
    }

    inline constexpr BlockType GetType() const
    {
        return m_Type;
    }

    void SetSkyLight(uint8_t light)
    {
        m_SkyLight = light;
    }

    void SetBlockLight(uint8_t light)
    {
        m_BlockLight = light;
    }

    void SetType(BlockType type)
    {
        m_Type = type;
    }

private:
    uint8_t m_SkyLight : 4 = 0;
    uint8_t m_BlockLight : 4 = 0;
    BlockType m_Type = BlockType::Air;
};
