#include "BlockSpecification.hh"

void BlockSpecification::SetAll(glm::uvec2 position)
{
    SetPosition(VertexDirection::Front, position);
    SetPosition(VertexDirection::Back, position);
    SetPosition(VertexDirection::Top, position);
    SetPosition(VertexDirection::Bottom, position);
    SetPosition(VertexDirection::Left, position);
    SetPosition(VertexDirection::Right, position);
}

void BlockSpecification::SetPosition(VertexDirection direction, glm::uvec2 position)
{
    Positions[(int) direction] = position;
}

const BlockSpecification &BlockSpecificationManager::GetSpecification(BlockType type) const
{
    return m_BlockSpecifications[(int) type];
}

void BlockSpecificationManager::Initialize()
{
    BlockSpecification blockSpecification;
    blockSpecification.Type = BlockType::Air;
    blockSpecification.SetAll(glm::uvec2(-1));
    AddSpecification(blockSpecification);

    blockSpecification = {};
    blockSpecification.Type = BlockType::Grass;
    blockSpecification.SetAll(glm::uvec2(3, 0));
    blockSpecification.SetPosition(VertexDirection::Top, glm::uvec2(0, 0));
    blockSpecification.SetPosition(VertexDirection::Bottom, glm::uvec2(2, 0));
    AddSpecification(blockSpecification);

    blockSpecification = {};
    blockSpecification.Type = BlockType::Dirt;
    blockSpecification.SetAll(glm::uvec2(2, 0));
    AddSpecification(blockSpecification);

    blockSpecification = {};
    blockSpecification.Type = BlockType::Stone;
    blockSpecification.SetAll(glm::uvec2(1, 0));
    AddSpecification(blockSpecification);

    blockSpecification = {};
    blockSpecification.Type = BlockType::Bedrock;
    blockSpecification.SetAll(glm::uvec2(1, 1));
    AddSpecification(blockSpecification);

    blockSpecification = {};
    blockSpecification.Type = BlockType::Water;
    blockSpecification.SetAll(glm::uvec2(8, 0));
    AddSpecification(blockSpecification);
}

void BlockSpecificationManager::AddSpecification(BlockSpecification blockSpecification)
{
    m_BlockSpecifications.emplace_back(blockSpecification);
}
