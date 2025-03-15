#include "Renderer.hh"

#include "BlockSpecification.hh"
#include "Utils/Logger.hh"

void Renderer::RenderBlock(FastNoiseLite &noise, const Block &block, glm::vec3 position, VertexBufferObject<VoxelVertex> &vertexBufferObject, std::vector<GLuint> &indices, GLuint &index) const
{
    constexpr float pf = 0.5f;
    constexpr float start = 0.5f;

    const BlockSpecification &specification = GetBlockSpecificationManager()->GetSpecification(block.Type);

    int triangleCount = 0;
    if (Chunk::GetBlockTypeAt(position.x, position.y, position.z + 1, noise) == BlockType::Air)
    {
        // front face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Front));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Front));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::TopRight, VoxelVertexDirection::Front));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Front));
    }

    if (Chunk::GetBlockTypeAt(position.x + 1, position.y, position.z, noise) == BlockType::Air)
    {
        // Right face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Right));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Right));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::TopRight, VoxelVertexDirection::Right));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Right));
    }

    if (Chunk::GetBlockTypeAt(position.x, position.y, position.z - 1, noise) == BlockType::Air)
    {
        // Back face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Back));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Back));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::TopRight, VoxelVertexDirection::Back));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Back));
    }

    if (Chunk::GetBlockTypeAt(position.x - 1, position.y, position.z, noise) == BlockType::Air)
    {
        // Left face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Left));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Left));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::TopRight, VoxelVertexDirection::Left));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Left));
    }

    if (Chunk::GetBlockTypeAt(position.x, position.y + 1, position.z, noise) == BlockType::Air)
    {
        // Top face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Top));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Top));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::TopRight, VoxelVertexDirection::Top));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Top));
    }

    if (Chunk::GetBlockTypeAt(position.x, position.y - 1, position.z, noise) == BlockType::Air)
    {
        // Bottom face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Bottom));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Bottom));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::TopRight, VoxelVertexDirection::Bottom));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Bottom));
    }

    for (int i = 0; i < triangleCount; ++i)
    {
        indices.push_back(index);
        indices.push_back(index + 2);
        indices.push_back(index + 1);
        indices.push_back(index);
        indices.push_back(index + 3);
        indices.push_back(index + 2);
        index += 4;
    }
}

void Renderer::RenderChunk(FastNoiseLite noise, Chunk &&chunk)
{
    LogDebug("Rendering chunk {} {} {}", chunk.Position.x, chunk.Position.y, chunk.Position.z);

    std::vector<GLuint> indices;
    indices.reserve(kChunkWidth * kChunkHeight * kChunkDepth * 24);

    GLuint index = 0;
    VertexBufferObject<VoxelVertex> vertexBufferObject;

    for (int x = 0; x < kChunkWidth; ++x)
    {
        for (int y = 0; y < kChunkHeight; ++y)
        {
            for (int z = 0; z < kChunkDepth; ++z)
            {
                if (chunk.GetBlockType(x, y, z) != BlockType::Air)
                {
                    glm::vec3 position = glm::vec3((float) x, (float) y, (float) z);
                    glm::vec3 chunkPosition = glm::vec3((float) chunk.GetPosition().x, 0.0, (float) chunk.GetPosition().y);

                    RenderBlock(noise, chunk.GetBlock(x, y, z), chunkPosition + position, vertexBufferObject, indices, index);
                }
            }
        }
    }

    RenderQueue renderQueue;
    renderQueue.Chunk = std::move(chunk);
    renderQueue.Indices = std::move(indices);
    renderQueue.VertexBufferObject = std::move(vertexBufferObject);

    std::unique_lock<std::shared_mutex> lock(m_QueueMutex);
    m_Queue.emplace_back(std::move(renderQueue));
}

void Renderer::DestroyChunk(GLuint vertexArrayObjectID)
{
    for (int i = 0; i < m_VertexArrayObjects.size(); ++i)
    {
        if (m_VertexArrayObjects[i].GetID() == vertexArrayObjectID)
        {
            m_VertexArrayObjects[i].Destroy();
            m_VertexArrayObjects.erase(m_VertexArrayObjects.begin() + i);
            break;
        }
    }
}

void Renderer::PollQueue(World &world)
{
    std::unique_lock<std::shared_mutex> lock(m_QueueMutex);
    for (auto &&[chunk, indices, vertexBufferObject] : m_Queue)
    {
        VertexArrayObject<VoxelVertex> vertexArrayObject;
        vertexArrayObject.Initialize();
        vertexArrayObject.Bind();

        vertexBufferObject.Initialize();
        vertexBufferObject.AddFloat(3);
        vertexBufferObject.AddUInteger(1);

        chunk.SetVertexArrayObjectID(vertexArrayObject.GetID());

        world.AddChunk(chunk);
        world.RemoveFuture(chunk.GetPosition());

        vertexArrayObject.Set(vertexBufferObject, indices);
        m_VertexArrayObjects.emplace_back(std::move(vertexArrayObject));

        LogDebug("Added chunk: ({},{})", chunk.Position.x, chunk.Position.y);
    }

    m_Queue.clear();
}
