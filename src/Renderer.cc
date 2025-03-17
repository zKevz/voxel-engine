#include "Renderer.hh"

#include "BlockSpecification.hh"
#include "Utils/Logger.hh"
#include "Utils/Utils.hh"

#include <GLFW/glfw3.h>

static constexpr float pf = 0.5f;
static constexpr float start = 0.5f;

enum VertexCornerPosition
{
    FrontLeftDown,
    FrontRightDown,
    FrontRightUp,
    FrontLeftUp,
    BackLeftDown,
    BackRightDown,
    BackRightUp,
    BackLeftUp,
};

void Renderer::RenderBlock(World *world, FastNoiseLite &noise, const Block &block, glm::vec3 position, VertexBufferObject<VoxelVertex> &vertexBufferObject, std::vector<GLuint> &indices, GLuint &index) const
{
    const BlockSpecification &specification = GetBlockSpecificationManager()->GetSpecification(block.GetType());

    const BlockSpecification *surroundingBlocks[6] = {
        &GetBlockSpecificationManager()->GetSpecification(world ? world->GetBlockType(position.x, position.y, position.z + 1) : Chunk::GetBlockTypeAt(position.x, position.y, position.z + 1, noise)),  // front
        &GetBlockSpecificationManager()->GetSpecification(world ? world->GetBlockType(position.x + 1, position.y, position.z) : Chunk::GetBlockTypeAt(position.x + 1, position.y, position.z, noise)),  // right
        &GetBlockSpecificationManager()->GetSpecification(world ? world->GetBlockType(position.x, position.y, position.z - 1) : Chunk::GetBlockTypeAt(position.x, position.y, position.z - 1, noise)),  // back
        &GetBlockSpecificationManager()->GetSpecification(world ? world->GetBlockType(position.x - 1, position.y, position.z) : Chunk::GetBlockTypeAt(position.x - 1, position.y, position.z, noise)),  // left
        &GetBlockSpecificationManager()->GetSpecification(world ? world->GetBlockType(position.x, position.y + 1, position.z) : Chunk::GetBlockTypeAt(position.x, position.y + 1, position.z, noise)),  // top
        &GetBlockSpecificationManager()->GetSpecification(world ? world->GetBlockType(position.x, position.y - 1, position.z) : Chunk::GetBlockTypeAt(position.x, position.y - 1, position.z, noise)),  // bottom
    };

    glm::vec3 vertexCornerPositions[8] = {
        glm::vec3(start - pf, start - pf, start + pf) + position,  // front left down
        glm::vec3(start + pf, start - pf, start + pf) + position,  // front right down
        glm::vec3(start + pf, start + pf, start + pf) + position,  // front right up
        glm::vec3(start - pf, start + pf, start + pf) + position,  // front left up
        glm::vec3(start - pf, start - pf, start - pf) + position,  // back left down
        glm::vec3(start + pf, start - pf, start - pf) + position,  // back right down
        glm::vec3(start + pf, start + pf, start - pf) + position,  // back right up
        glm::vec3(start - pf, start + pf, start - pf) + position,  // back left up
    };

    // for water
    if (block.GetType() == BlockType::Water)
    {
        // we want to decrease the "block height"
        float depth = 0.1f;
        vertexCornerPositions[FrontLeftUp].y -= depth;
        vertexCornerPositions[FrontRightUp].y -= depth;
        vertexCornerPositions[BackLeftUp].y -= depth;
        vertexCornerPositions[BackRightUp].y -= depth;
    }

    auto ShouldDrawThisFace = [&](int index) {
        if (!surroundingBlocks[index]->IsTransparent())
        {
            return false;
        }

        if (block.GetType() == BlockType::Water && surroundingBlocks[index]->Type != BlockType::Air)
        {
            return false;
        }

        return true;
    };

    int triangleCount = 0;
    if (ShouldDrawThisFace(0))
    {
        // front face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontLeftDown], specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Front, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontRightDown], specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Front, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontRightUp], specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::TopRight, VoxelVertexDirection::Front, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontLeftUp], specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Front, position, block.GetSkyLight(), block.GetBlockLight()));
    }

    if (ShouldDrawThisFace(1))
    {
        // Right face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontRightDown], specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Right, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackRightDown], specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Right, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackRightUp], specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::TopRight, VoxelVertexDirection::Right, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontRightUp], specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Right, position, block.GetSkyLight(), block.GetBlockLight()));
    }

    if (ShouldDrawThisFace(2))
    {
        // Back face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackRightDown], specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Back, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackLeftDown], specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Back, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackLeftUp], specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::TopRight, VoxelVertexDirection::Back, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackRightUp], specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Back, position, block.GetSkyLight(), block.GetBlockLight()));
    }

    if (ShouldDrawThisFace(3))
    {
        // Left face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackLeftDown], specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Left, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontLeftDown], specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Left, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontLeftUp], specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::TopRight, VoxelVertexDirection::Left, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackLeftUp], specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Left, position, block.GetSkyLight(), block.GetBlockLight()));
    }

    if (ShouldDrawThisFace(4))
    {
        if (block.GetType() != BlockType::Water || surroundingBlocks[4]->Type == BlockType::Air)
        {
            // Top face
            ++triangleCount;
            vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontLeftUp], specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Top, position, block.GetSkyLight(), block.GetBlockLight()));
            vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontRightUp], specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Top, position, block.GetSkyLight(), block.GetBlockLight()));
            vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackRightUp], specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::TopRight, VoxelVertexDirection::Top, position, block.GetSkyLight(), block.GetBlockLight()));
            vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackLeftUp], specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Top, position, block.GetSkyLight(), block.GetBlockLight()));
        }
    }

    if (ShouldDrawThisFace(5))
    {
        // Bottom face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackLeftDown], specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Bottom, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[BackRightDown], specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Bottom, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontRightDown], specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::TopRight, VoxelVertexDirection::Bottom, position, block.GetSkyLight(), block.GetBlockLight()));
        vertexBufferObject.AddVertex(VoxelVertex(vertexCornerPositions[FrontLeftDown], specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Bottom, position, block.GetSkyLight(), block.GetBlockLight()));
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

    std::vector<GLuint> voxelIndices;
    std::vector<GLuint> waterIndices;
    voxelIndices.reserve(kChunkWidth * kChunkHeight * kChunkDepth * 24);
    waterIndices.reserve(kChunkWidth * kChunkHeight * kChunkDepth * 24);

    GLuint voxelIndex = 0;
    GLuint waterIndex = 0;
    VertexBufferObject<VoxelVertex> voxelVertexBufferObject;
    VertexBufferObject<VoxelVertex> waterVertexBufferObject;

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

                    if (chunk.GetBlockType(x, y, z) == BlockType::Water)
                    {
                        RenderBlock(nullptr, noise, chunk.GetBlock(x, y, z), chunkPosition + position, waterVertexBufferObject, waterIndices, waterIndex);
                    }
                    else
                    {
                        RenderBlock(nullptr, noise, chunk.GetBlock(x, y, z), chunkPosition + position, voxelVertexBufferObject, voxelIndices, voxelIndex);
                    }
                }
            }
        }
    }

    RenderQueue renderQueue;
    renderQueue.Chunk = std::move(chunk);
    renderQueue.WaterIndices = std::move(waterIndices);
    renderQueue.VoxelIndices = std::move(voxelIndices);
    renderQueue.WaterVertexBufferObject = std::move(waterVertexBufferObject);
    renderQueue.VoxelVertexBufferObject = std::move(voxelVertexBufferObject);

    std::unique_lock<std::shared_mutex> lock(m_QueueMutex);
    m_Queue.emplace_back(std::move(renderQueue));
}

void Renderer::ReRenderChunk(World &world, Chunk &chunk, FastNoiseLite &noise)
{
    LogDebug("re-rendering chunk ({},{})", chunk.GetPosition().x, chunk.GetPosition().y);

    std::vector<GLuint> indices;
    indices.reserve(kChunkWidth * kChunkHeight * kChunkDepth * 24);

    chunk.InitializeSkyLight();
    chunk.InitializeBlockLight();

    VertexArrayObject<VoxelVertex> &voxelVertexArrayObject = GetVoxelVertexArrayObjectByID(chunk.GetVertexArrayObjectID());
    voxelVertexArrayObject.DestroyInner();
    voxelVertexArrayObject.Bind();

    GLuint index = 0;
    VertexBufferObject<VoxelVertex> vertexBufferObject;
    vertexBufferObject.Initialize();
    vertexBufferObject.AddFloat(3);
    vertexBufferObject.AddUInteger(1);
    vertexBufferObject.AddFloat(3);

    for (int x = 0; x < kChunkWidth; ++x)
    {
        for (int y = 0; y < kChunkHeight; ++y)
        {
            for (int z = 0; z < kChunkDepth; ++z)
            {
                if (chunk.GetBlockType(x, y, z) != BlockType::Air && chunk.GetBlockType(x, y, z) != BlockType::Water)
                {
                    glm::vec3 position = glm::vec3((float) x, (float) y, (float) z);
                    glm::vec3 chunkPosition = glm::vec3((float) chunk.GetPosition().x, 0.0, (float) chunk.GetPosition().y);

                    RenderBlock(&world, noise, chunk.GetBlock(x, y, z), chunkPosition + position, vertexBufferObject, indices, index);
                }
            }
        }
    }

    vertexBufferObject.Build();
    voxelVertexArrayObject.Set(vertexBufferObject, indices);

    VertexArrayObject<VoxelVertex> &waterVertexArrayObject = GetWaterVertexArrayObjectByID(chunk.GetVertexArrayObjectID());
    waterVertexArrayObject.DestroyInner();
    waterVertexArrayObject.Bind();

    index = 0;
    indices = {};
    indices.reserve(kChunkWidth * kChunkHeight * kChunkDepth * 24);
    vertexBufferObject = {};
    vertexBufferObject.Initialize();
    vertexBufferObject.AddFloat(3);
    vertexBufferObject.AddUInteger(1);
    vertexBufferObject.AddFloat(3);

    for (int x = 0; x < kChunkWidth; ++x)
    {
        for (int y = 0; y < kChunkHeight; ++y)
        {
            for (int z = 0; z < kChunkDepth; ++z)
            {
                if (chunk.GetBlockType(x, y, z) == BlockType::Water)
                {
                    glm::vec3 position = glm::vec3((float) x, (float) y, (float) z);
                    glm::vec3 chunkPosition = glm::vec3((float) chunk.GetPosition().x, 0.0, (float) chunk.GetPosition().y);

                    RenderBlock(&world, noise, chunk.GetBlock(x, y, z), chunkPosition + position, vertexBufferObject, indices, index);
                }
            }
        }
    }

    vertexBufferObject.Build();
    waterVertexArrayObject.Set(vertexBufferObject, indices);
}

void Renderer::DestroyChunk(GLuint vertexArrayObjectID)
{
    for (int i = 0; i < m_VoxelVertexArrayObjects.size(); ++i)
    {
        if (m_VoxelVertexArrayObjects[i].GetID() == vertexArrayObjectID)
        {
            m_VoxelVertexArrayObjects[i].Destroy();
            m_WaterVertexArrayObjects[i].Destroy();
            m_VoxelVertexArrayObjects.erase(m_VoxelVertexArrayObjects.begin() + i);
            m_WaterVertexArrayObjects.erase(m_WaterVertexArrayObjects.begin() + i);
            break;
        }
    }
}

void Renderer::PollQueue(World &world)
{
    std::unique_lock<std::shared_mutex> lock(m_QueueMutex);
    for (auto &&renderQueue : m_Queue)
    {
        VertexArrayObject<VoxelVertex> voxelVertexArrayObject;
        voxelVertexArrayObject.Initialize();
        voxelVertexArrayObject.Bind();

        renderQueue.VoxelVertexBufferObject.Initialize();
        renderQueue.VoxelVertexBufferObject.AddFloat(3);
        renderQueue.VoxelVertexBufferObject.AddUInteger(1);
        renderQueue.VoxelVertexBufferObject.AddFloat(3);

        voxelVertexArrayObject.Set(renderQueue.VoxelVertexBufferObject, renderQueue.VoxelIndices);
        m_VoxelVertexArrayObjects.emplace_back(std::move(voxelVertexArrayObject));

        VertexArrayObject<VoxelVertex> waterVertexArrayObject;
        waterVertexArrayObject.Initialize();
        waterVertexArrayObject.Bind();

        renderQueue.WaterVertexBufferObject.Initialize();
        renderQueue.WaterVertexBufferObject.AddFloat(3);
        renderQueue.WaterVertexBufferObject.AddUInteger(1);
        renderQueue.WaterVertexBufferObject.AddFloat(3);

        waterVertexArrayObject.Set(renderQueue.WaterVertexBufferObject, renderQueue.WaterIndices);
        m_WaterVertexArrayObjects.emplace_back(std::move(waterVertexArrayObject));

        renderQueue.Chunk.SetVertexArrayObjectID(voxelVertexArrayObject.GetID());

        world.RemoveQueue(renderQueue.Chunk.GetPosition());
        world.AddChunk(std::move(renderQueue.Chunk));

        LogDebug("Added chunk: ({},{})", chunk.Position.x, chunk.Position.y);
    }

    m_Queue.clear();
}

void Renderer::Destroy()
{
    m_VoxelShader.Destroy();
    m_CrossHairShader.Destroy();

    for (auto &&vertexArrayObject : GetVertexArrayObjects())
    {
        vertexArrayObject.Destroy();
    }
}

void Renderer::Render(World &world, const Camera3D &camera)
{
    RenderWorld(world, camera);
    RenderCrossHair();
    RenderSkyBox(camera);
}

void Renderer::RenderWorld(World &world, const Camera3D &camera)
{
    glm::mat4 proj = glm::perspective(glm::radians(camera.GetZoom()), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 10000.0f);
    glm::mat4 view = camera.GetViewMatrix();
    static glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    glm::mat4 mvp = proj * view * model;

    m_VoxelTexture.Bind();
    m_VoxelShader.Bind();
    m_VoxelShader.SetUniform("u_Mvp", mvp);
    m_VoxelShader.SetUniform("u_Time", (float) glfwGetTime());
    m_VoxelShader.SetUniform("u_Texture", 0);
    m_VoxelShader.SetUniform("u_Ambient", m_Ambient);
    m_VoxelShader.SetUniform("u_LightDirection", m_LightSourceDirection);

    RaycastBlockResult raycastBlockResult = Utils::RaycastBlock(camera.GetPosition(), camera.GetFront(), world);
    m_VoxelShader.SetUniform("u_Raycast", glm::vec4((float) raycastBlockResult.BlockPosition.x, (float) raycastBlockResult.BlockPosition.y, (float) raycastBlockResult.BlockPosition.z, (float) raycastBlockResult.Hit));

    for (int i = 0; i < m_VoxelVertexArrayObjects.size(); ++i)
    {
        const VertexArrayObject<VoxelVertex> &voxelVertexArrayObject = m_VoxelVertexArrayObjects[i];
        const VertexArrayObject<VoxelVertex> &waterVertexArrayObject = m_WaterVertexArrayObjects[i];

        voxelVertexArrayObject.Bind();

        glDisable(GL_BLEND);
        glDrawElements(GL_TRIANGLES, voxelVertexArrayObject.GetIndicesCount(), GL_UNSIGNED_INT, 0);

        waterVertexArrayObject.Bind();

        glEnable(GL_BLEND);
        glDrawElements(GL_TRIANGLES, waterVertexArrayObject.GetIndicesCount(), GL_UNSIGNED_INT, 0);
    }
}

void Renderer::InitializeSkyBox()
{
    m_SkyBoxVertexArrayObject.Initialize();
    m_SkyBoxVertexArrayObject.Bind();

    VertexBufferObject<SkyBoxVertex> skyBoxVertexBufferObject;
    skyBoxVertexBufferObject.Initialize();
    skyBoxVertexBufferObject.AddFloat(3);

    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, 1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, -1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, -1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, 1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, 1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, -1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, 1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, 1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, 1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, -1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, -1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, -1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, 1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, 1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, 1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, -1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, -1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, 1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, 1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, 1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, -1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, -1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, 1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, 1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, 1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, 1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, 1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, 1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, -1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, -1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, -1.0f, -1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(-1.0f, -1.0f, 1.0f)));
    skyBoxVertexBufferObject.AddVertex(SkyBoxVertex(glm::vec3(1.0f, -1.0f, 1.0f)));
    skyBoxVertexBufferObject.Build();

    m_SkyBoxVertexArrayObject.SetVertexBufferObject(skyBoxVertexBufferObject);
}

void Renderer::InitializeCrossHair()
{
    m_CrossHairVertexArrayObject.Initialize();
    m_CrossHairVertexArrayObject.Bind();

    VertexBufferObject<CrossHairVertex> crossHairVertexBufferObject;
    crossHairVertexBufferObject.Initialize();
    crossHairVertexBufferObject.AddFloat(2);

    // Define your crosshair size parameters
    float crossHairLength = 0.01f;  // Half-length of the bar
    float crossHairWidth = 0.001f;  // Half-thickness of the bar (width)

    // Horizontal bar (centered at origin)
    crossHairVertexBufferObject.AddVertex(CrossHairVertex(glm::vec2(-crossHairLength, crossHairWidth)));   // Left Top
    crossHairVertexBufferObject.AddVertex(CrossHairVertex(glm::vec2(crossHairLength, crossHairWidth)));    // Right Top
    crossHairVertexBufferObject.AddVertex(CrossHairVertex(glm::vec2(crossHairLength, -crossHairWidth)));   // Right Bottom
    crossHairVertexBufferObject.AddVertex(CrossHairVertex(glm::vec2(-crossHairLength, -crossHairWidth)));  // Left Bottom

    // Vertical bar (centered at origin)
    crossHairVertexBufferObject.AddVertex(CrossHairVertex(glm::vec2(-crossHairWidth, crossHairLength)));   // Top Left
    crossHairVertexBufferObject.AddVertex(CrossHairVertex(glm::vec2(crossHairWidth, crossHairLength)));    // Top Right
    crossHairVertexBufferObject.AddVertex(CrossHairVertex(glm::vec2(crossHairWidth, -crossHairLength)));   // Bottom Right
    crossHairVertexBufferObject.AddVertex(CrossHairVertex(glm::vec2(-crossHairWidth, -crossHairLength)));  // Bottom Left

    m_CrossHairVertexArrayObject.Set(crossHairVertexBufferObject, { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4 });
    m_CrossHairVertexArrayObject.Unbind();
}

void Renderer::InitializeSkyBoxTexture(TextureCubeMapPath path)
{
    m_SkyBoxTexture.Destroy();
    m_SkyBoxTexture.Initialize(path);
}

void Renderer::InitializeSkyBoxTextureCool()
{
    TextureCubeMapPath textureCubeMapPath;
    textureCubeMapPath.RightPath = "resources/textures/clouds1/clouds1_east.bmp";
    textureCubeMapPath.LeftPath = "resources/textures/clouds1/clouds1_west.bmp";
    textureCubeMapPath.UpPath = "resources/textures/clouds1/clouds1_up.bmp";
    textureCubeMapPath.DownPath = "resources/textures/clouds1/clouds1_down.bmp";
    textureCubeMapPath.FrontPath = "resources/textures/clouds1/clouds1_north.bmp";
    textureCubeMapPath.BackPath = "resources/textures/clouds1/clouds1_south.bmp";
    InitializeSkyBoxTexture(textureCubeMapPath);
}

void Renderer::InitializeSkyBoxTexture(std::string_view type)
{
    TextureCubeMapPath textureCubeMapPath;
    textureCubeMapPath.RightPath = fmt::format("resources/textures/cloudy/{}cloud_rt.jpg", type);
    textureCubeMapPath.LeftPath = fmt::format("resources/textures/cloudy/{}cloud_lf.jpg", type);
    textureCubeMapPath.UpPath = fmt::format("resources/textures/cloudy/{}cloud_up.jpg", type);
    textureCubeMapPath.DownPath = fmt::format("resources/textures/cloudy/{}cloud_dn.jpg", type);
    textureCubeMapPath.FrontPath = fmt::format("resources/textures/cloudy/{}cloud_ft.jpg", type);
    textureCubeMapPath.BackPath = fmt::format("resources/textures/cloudy/{}cloud_bk.jpg", type);
    InitializeSkyBoxTexture(textureCubeMapPath);
}

void Renderer::Initialize()
{
    m_VoxelShader.Initialize("resources/shaders/voxel_vertex.glsl.vert", "resources/shaders/voxel_fragment.glsl.frag");
    m_SkyBoxShader.Initialize("resources/shaders/skybox_vertex.glsl.vert", "resources/shaders/skybox_fragment.glsl.frag");
    m_CrossHairShader.Initialize("resources/shaders/crosshair_vertex.glsl.vert", "resources/shaders/crosshair_fragment.glsl.frag");

    m_VoxelTexture.Initialize("resources/textures/terrain.png");

    // InitializeSkyBoxTexture("blue");
    InitializeSkyBoxTextureCool();
    InitializeCrossHair();
    InitializeSkyBox();
}

void Renderer::RenderCrossHair()
{
    m_CrossHairShader.Bind();
    m_CrossHairVertexArrayObject.Bind();
    glDrawElements(GL_TRIANGLES, m_CrossHairVertexArrayObject.GetIndicesCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::RenderSkyBox(const Camera3D &camera)
{
    m_SkyBoxVertexArrayObject.Bind();

    GLint oldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);

    GLint oldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 proj = glm::perspective(glm::radians(camera.GetZoom()), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 10000.0f);
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));  // remove translation from the view matrix

    m_SkyBoxTexture.Bind(1);
    m_SkyBoxShader.Bind();
    m_SkyBoxShader.SetUniform("u_Projection", proj);
    m_SkyBoxShader.SetUniform("u_View", view);
    m_SkyBoxShader.SetUniform("u_Texture", 1);

    glDrawArrays(GL_TRIANGLES, 0, m_SkyBoxVertexArrayObject.GetIndicesCount());
    glCullFace(oldCullFaceMode);
    glDepthFunc(oldDepthFuncMode);
}
