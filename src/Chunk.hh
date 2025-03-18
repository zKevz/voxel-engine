#pragma once
#include "Block.hh"
#include "OpenGL/Camera3D.hh"

#include <FastNoiseLite/FastNoiseLite.h>

static constexpr int kChunkDepth = 16;
static constexpr int kChunkWidth = 16;
static constexpr int kChunkHeight = 384;  // 256;

class Chunk
{
public:
    Chunk();

    void Initialize(const FastNoiseLite &noise, glm::ivec2 position);
    void InitializeSkyLight();
    void InitializeBlockLight();

    inline constexpr glm::ivec2 GetPosition() const
    {
        return m_Position;
    }

    inline constexpr Block &GetBlock(int x, int y, int z)
    {
        assert(x >= 0 && x < kChunkWidth);
        assert(y >= 0 && y < kChunkHeight);
        assert(z >= 0 && z < kChunkDepth);
        return m_Blocks[x][y][z];
    }

    inline constexpr Block *GetBlockSafe(int x, int y, int z)
    {
        if (x < 0 || x > kChunkWidth || y < 0 || y > kChunkHeight || z < 0 || z > kChunkDepth)
        {
            return nullptr;
        }

        return &m_Blocks[x][y][z];
    }

    inline constexpr BlockType GetBlockType(int x, int y, int z) const
    {
        assert(x >= 0 && x < kChunkWidth);
        assert(y >= 0 && y < kChunkHeight);
        assert(z >= 0 && z < kChunkDepth);
        return m_Blocks[x][y][z].GetType();
    }

    inline constexpr GLuint GetVertexArrayObjectID() const
    {
        return m_VertexArrayObjectID;
    }

    void SetVertexArrayObjectID(GLuint id)
    {
        m_VertexArrayObjectID = id;
    }

    static bool IsTransparent(int x, int y, int z, const FastNoiseLite &noise);
    static BlockType GetBlockTypeAt(int x, int y, int z, const FastNoiseLite &noise);

private:
    void AddFeatures(const FastNoiseLite &noise);
    void PlaceTree(int x, int y, int z);

    GLuint m_VertexArrayObjectID;
    glm::ivec2 m_Position;

    std::vector<std::vector<std::vector<Block>>> m_Blocks;
    // std::array<std::array<std::array<Block, kChunkDepth>, kChunkHeight>, kChunkWidth> m_Blocks;
};
