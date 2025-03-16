#pragma once
#include "Block.hh"
#include "OpenGL/Camera3D.hh"

#include <FastNoiseLite/FastNoiseLite.h>

#include <array>

static constexpr int kChunkDepth = 16;
static constexpr int kChunkWidth = 16;
static constexpr int kChunkHeight = 256;

class Chunk
{
public:
    void Initialize(const FastNoiseLite &noise, glm::ivec2 position);

    inline constexpr glm::ivec2 GetPosition() const
    {
        return m_Position;
    }

    inline constexpr Block &GetBlock(int x, int y, int z)
    {
        return m_Blocks[x][y][z];
    }

    inline constexpr BlockType GetBlockType(int x, int y, int z) const
    {
        return m_Blocks[x][y][z].Type;
    }

    inline constexpr GLuint GetVertexArrayObjectID() const
    {
        return m_VertexArrayObjectID;
    }

    void SetVertexArrayObjectID(GLuint id)
    {
        m_VertexArrayObjectID = id;
    }

    static BlockType GetBlockTypeAt(int x, int y, int z, const FastNoiseLite &noise);

private:
    GLuint m_VertexArrayObjectID;
    glm::ivec2 m_Position;
    std::array<std::array<std::array<Block, kChunkDepth>, kChunkHeight>, kChunkWidth> m_Blocks;
};
