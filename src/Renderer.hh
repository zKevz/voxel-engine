#pragma once

#include "OpenGL/VertexArrayObject.hh"
#include "OpenGL/VoxelVertex.hh"
#include "World.hh"

#include <shared_mutex>

class Renderer
{
public:
    void RenderBlock(FastNoiseLite &noise, const Block &block, glm::vec3 position, VertexBufferObject<VoxelVertex> &vertexBufferObject, std::vector<GLuint> &indices, GLuint &index) const;
    void RenderChunk(FastNoiseLite noise, Chunk &&chunk);
    void DestroyChunk(GLuint vertexArrayObjectID);
    void PollQueue(World &world);

    inline constexpr const std::vector<VertexArrayObject<VoxelVertex>> &GetVertexArrayObjects() const
    {
        return m_VertexArrayObjects;
    }

private:
    struct RenderQueue
    {
        Chunk Chunk;
        std::vector<GLuint> Indices;
        VertexBufferObject<VoxelVertex> VertexBufferObject;
    };

    std::shared_mutex m_QueueMutex;
    std::vector<RenderQueue> m_Queue;
    std::vector<VertexArrayObject<VoxelVertex>> m_VertexArrayObjects;
};
