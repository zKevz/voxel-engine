#include "Renderer.hh"

#include "BlockSpecification.hh"
#include "Utils/Logger.hh"
#include "Utils/Utils.hh"

static constexpr float pf = 0.5f;
static constexpr float start = 0.5f;

void Renderer::RenderBlock(World *world, FastNoiseLite &noise, const Block &block, glm::vec3 position, VertexBufferObject<VoxelVertex> &vertexBufferObject, std::vector<GLuint> &indices, GLuint &index) const
{
    const BlockSpecification &specification = GetBlockSpecificationManager()->GetSpecification(block.Type);

    BlockType surroundingBlocks[6] = {
        world ? world->GetBlockType(position.x, position.y, position.z + 1) : Chunk::GetBlockTypeAt(position.x, position.y, position.z + 1, noise),  // front
        world ? world->GetBlockType(position.x + 1, position.y, position.z) : Chunk::GetBlockTypeAt(position.x + 1, position.y, position.z, noise),  // right
        world ? world->GetBlockType(position.x, position.y, position.z - 1) : Chunk::GetBlockTypeAt(position.x, position.y, position.z - 1, noise),  // back
        world ? world->GetBlockType(position.x - 1, position.y, position.z) : Chunk::GetBlockTypeAt(position.x - 1, position.y, position.z, noise),  // left
        world ? world->GetBlockType(position.x, position.y + 1, position.z) : Chunk::GetBlockTypeAt(position.x, position.y + 1, position.z, noise),  // top
        world ? world->GetBlockType(position.x, position.y - 1, position.z) : Chunk::GetBlockTypeAt(position.x, position.y - 1, position.z, noise),  // bottom
    };

    int triangleCount = 0;
    if (surroundingBlocks[0] == BlockType::Air)
    {
        // front face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Front, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Front, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::TopRight, VoxelVertexDirection::Front, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Front), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Front, position));
    }

    if (surroundingBlocks[1] == BlockType::Air)
    {
        // Right face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Right, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Right, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::TopRight, VoxelVertexDirection::Right, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Right), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Right, position));
    }

    if (surroundingBlocks[2] == BlockType::Air)
    {
        // Back face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Back, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Back, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::TopRight, VoxelVertexDirection::Back, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Back), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Back, position));
    }

    if (surroundingBlocks[3] == BlockType::Air)
    {
        // Left face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Left, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Left, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::TopRight, VoxelVertexDirection::Left, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Left), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Left, position));
    }

    if (surroundingBlocks[4] == BlockType::Air)
    {
        // Top face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Top, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Top, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::TopRight, VoxelVertexDirection::Top, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start + pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Top), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Top, position));
    }

    if (surroundingBlocks[5] == BlockType::Air)
    {
        // Bottom face
        ++triangleCount;
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::BottomLeft, VoxelVertexDirection::Bottom, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start - pf + position.z), specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::BottomRight, VoxelVertexDirection::Bottom, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start + pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::TopRight, VoxelVertexDirection::Bottom, position));
        vertexBufferObject.AddVertex(VoxelVertex(glm::vec3(start - pf + position.x, start - pf + position.y, start + pf + position.z), specification.GetPosition(VoxelVertexDirection::Bottom), VoxelVertexUVType::TopLeft, VoxelVertexDirection::Bottom, position));
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

                    RenderBlock(nullptr, noise, chunk.GetBlock(x, y, z), chunkPosition + position, vertexBufferObject, indices, index);
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

void Renderer::ReRenderChunk(World &world, Chunk &chunk, FastNoiseLite &noise)
{
    LogDebug("re-rendering chunk ({},{})", chunk.GetPosition().x, chunk.GetPosition().y);

    std::vector<GLuint> indices;
    indices.reserve(kChunkWidth * kChunkHeight * kChunkDepth * 24);

    VertexArrayObject<VoxelVertex> &vertexArrayObject = GetVertexArrayObjectByID(chunk.GetVertexArrayObjectID());
    vertexArrayObject.DestroyInner();
    vertexArrayObject.Bind();

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
                if (chunk.GetBlockType(x, y, z) != BlockType::Air)
                {
                    glm::vec3 position = glm::vec3((float) x, (float) y, (float) z);
                    glm::vec3 chunkPosition = glm::vec3((float) chunk.GetPosition().x, 0.0, (float) chunk.GetPosition().y);

                    RenderBlock(&world, noise, chunk.GetBlock(x, y, z), chunkPosition + position, vertexBufferObject, indices, index);
                }
            }
        }
    }

    vertexBufferObject.Build();
    vertexArrayObject.Set(vertexBufferObject, indices);
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
        vertexBufferObject.AddFloat(3);

        chunk.SetVertexArrayObjectID(vertexArrayObject.GetID());

        world.AddChunk(chunk);
        world.RemoveFuture(chunk.GetPosition());

        vertexArrayObject.Set(vertexBufferObject, indices);
        m_VertexArrayObjects.emplace_back(std::move(vertexArrayObject));

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
    glm::mat4 proj = glm::perspective(glm::radians(camera.GetZoom()), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = camera.GetViewMatrix();
    static glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    glm::mat4 mvp = proj * view * model;

    m_VoxelTexture.Bind();
    m_VoxelShader.Bind();
    m_VoxelShader.SetUniform("u_Mvp", mvp);
    m_VoxelShader.SetUniform("u_Texture", 0);

    RaycastBlockResult raycastBlockResult = Utils::RaycastBlock(camera.GetPosition(), camera.GetFront(), world);
    m_VoxelShader.SetUniform("u_Raycast", glm::vec4((float) raycastBlockResult.BlockPosition.x, (float) raycastBlockResult.BlockPosition.y, (float) raycastBlockResult.BlockPosition.z, (float) raycastBlockResult.Hit));

    for (const VertexArrayObject<VoxelVertex> &vertexArrayObject : GetVertexArrayObjects())
    {
        vertexArrayObject.Bind();
        glDrawElements(GL_TRIANGLES, vertexArrayObject.GetIndicesCount(), GL_UNSIGNED_INT, 0);
    }

    RenderCrossHair();
}

void Renderer::RenderCrossHair()
{
    m_CrossHairShader.Bind();
    m_CrossHairVertexArrayObject.Bind();
    glDrawElements(GL_TRIANGLES, m_CrossHairVertexArrayObject.GetIndicesCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::Initialize()
{
    m_VoxelShader.Initialize("resources/shaders/voxel_vertex.glsl.vert", "resources/shaders/voxel_fragment.glsl.frag");
    m_CrossHairShader.Initialize("resources/shaders/crosshair_vertex.glsl.vert", "resources/shaders/crosshair_fragment.glsl.frag");

    m_VoxelTexture.Initialize("resources/textures/terrain.png");

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
