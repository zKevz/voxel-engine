#pragma once

#include "OpenGL/Shader.hh"
#include "OpenGL/Texture.hh"
#include "OpenGL/TextureCubeMap.hh"
#include "OpenGL/VertexArrayObject.hh"
#include "Vertex/CrossHairVertex.hh"
#include "Vertex/SkyBoxVertex.hh"
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
    void Initialize();
    void InitializeSkyBoxTexture(TextureCubeMapPath path);
    void InitializeSkyBoxTexture(std::string_view type);
    void InitializeSkyBoxTextureCool();  // idk why i name it this..

    inline constexpr const std::vector<VertexArrayObject<VoxelVertex>> &GetVertexArrayObjects() const
    {
        return m_VoxelVertexArrayObjects;
    }

    inline constexpr VertexArrayObject<VoxelVertex> &GetVoxelVertexArrayObjectByID(GLuint id)
    {
        return *std::find_if(m_VoxelVertexArrayObjects.begin(), m_VoxelVertexArrayObjects.end(), [=](const auto &a) {
            return a.GetID() == id;
        });
    }

    inline constexpr VertexArrayObject<VoxelVertex> &GetWaterVertexArrayObjectByID(GLuint id)
    {
        for (int i = 0; i < m_VoxelVertexArrayObjects.size(); ++i)
        {
            if (m_VoxelVertexArrayObjects[i].GetID() == id)
            {
                return m_WaterVertexArrayObjects[i];
            }
        }

        throw "?!?!?!?!";
    }

    inline constexpr float *GetAmbientPointer()
    {
        return &m_Ambient;
    }

    inline constexpr glm::vec3 *GetLightSourceDirectionPointer()
    {
        return &m_LightSourceDirection;
    }

private:
    void InitializeSkyBox();
    void InitializeCrossHair();

    void RenderWorld(World &world, const Camera3D &camera);
    void RenderSkyBox(const Camera3D &camera);
    void RenderCrossHair();

    struct RenderQueue
    {
        Chunk Chunk;

        std::vector<GLuint> WaterIndices;
        std::vector<GLuint> VoxelIndices;

        VertexBufferObject<VoxelVertex> VoxelVertexBufferObject;
        VertexBufferObject<VoxelVertex> WaterVertexBufferObject;
    };

    float m_Ambient = 0.7f;
    glm::vec3 m_LightSourceDirection = glm::vec3(-1.0, 1.0, 1.0);

    Shader m_VoxelShader;
    Shader m_SkyBoxShader;
    Shader m_CrossHairShader;

    Texture m_VoxelTexture;
    TextureCubeMap m_SkyBoxTexture;

    mutable std::shared_mutex m_QueueMutex;
    std::vector<RenderQueue> m_Queue;

    std::vector<VertexArrayObject<VoxelVertex>> m_VoxelVertexArrayObjects;
    std::vector<VertexArrayObject<VoxelVertex>> m_WaterVertexArrayObjects;

    VertexArrayObject<SkyBoxVertex> m_SkyBoxVertexArrayObject;
    VertexArrayObject<CrossHairVertex> m_CrossHairVertexArrayObject;
};
