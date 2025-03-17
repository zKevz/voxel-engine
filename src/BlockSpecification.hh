#pragma once
#include "BlockType.hh"
#include "Vertex/VoxelVertex.hh"

#include <glm/glm.hpp>

#include <vector>

struct BlockSpecification
{
    BlockType Type;
    glm::uvec2 Positions[(int) VoxelVertexDirection::Count];

    void SetAll(glm::uvec2 position);
    void SetPosition(VoxelVertexDirection direction, glm::uvec2 position);

    bool IsTransparent() const;

    inline constexpr glm::uvec2 GetPosition(VoxelVertexDirection direction) const
    {
        return Positions[(int) direction];
    }
};

class BlockSpecificationManager
{
public:
    void Initialize();

    const BlockSpecification &GetSpecification(BlockType type) const;

private:
    void AddSpecification(BlockSpecification blockSpecification);

    std::vector<BlockSpecification> m_BlockSpecifications;
};

inline BlockSpecificationManager *GetBlockSpecificationManager()
{
    static BlockSpecificationManager instance;
    return &instance;
}
