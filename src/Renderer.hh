#pragma once

#include "OpenGL/Shader.hh"
#include "OpenGL/Texture.hh"
#include "OpenGL/VertexArrayObject.hh"
#include "Vertex/CrossHairVertex.hh"
#include "Vertex/VoxelVertex.hh"
#include "World.hh"

#include <shared_mutex>

constexpr const int SCREEN_WIDTH = 1200;
constexpr const int SCREEN_HEIGHT = 800;

class Renderer
{
public:
    void RenderBlock(World *world, FastNoiseLite &noise, const Block &block, glm::vec3 position, VertexBufferObject<VoxelVertex> &vertexBufferObject, std::vector<GLuint> &indices, GLuint &index) const;
    void RenderChunk(FastNoiseLite noise, Chunk &&chunk);
    void DestroyChunk(GLuint vertexArrayObjectID);
    void PollQueue(World &world);

    void Render(World &world, const Camera3D &camera);
    void ReRenderChunk(World &world, Chunk &chunk, FastNoiseLite &noise);
    void Destroy();
    void RenderCrossHair();
    void Initialize();

    inline constexpr const std::vector<VertexArrayObject<VoxelVertex>> &GetVertexArrayObjects() const
    {
        return m_VertexArrayObjects;
    }

    inline constexpr VertexArrayObject<VoxelVertex> &GetVertexArrayObjectByID(GLuint id)
    {
        return *std::find_if(m_VertexArrayObjects.begin(), m_VertexArrayObjects.end(), [=](const auto &a) {
            return a.GetID() == id;
        });
    }

private:
    struct RenderQueue
    {
        Chunk Chunk;
        std::vector<GLuint> Indices;
        VertexBufferObject<VoxelVertex> VertexBufferObject;
    };

    Shader m_VoxelShader;
    Shader m_CrossHairShader;

    Texture m_VoxelTexture;

    std::shared_mutex m_QueueMutex;
    std::vector<RenderQueue> m_Queue;
    std::vector<VertexArrayObject<VoxelVertex>> m_VertexArrayObjects;

    VertexArrayObject<CrossHairVertex> m_CrossHairVertexArrayObject;
};
