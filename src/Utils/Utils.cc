#include "Utils.hh"

#include "Logger.hh"

#include <fstream>

std::string Utils::ReadFile(std::string_view path)
{
    std::fstream fs(path);
    if (!fs)
    {
        LogError("Cannot read file {}", path);
        return "";
    }

    std::string string((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    return string;
}

RaycastBlockResult Utils::RaycastBlock(glm::vec3 origin, glm::vec3 direction, World &world, float maxDistance, float stepSize)
{
    glm::vec3 rayOrigin = origin;                  // camera.GetPosition();
    glm::vec3 rayDir = glm::normalize(direction);  // glm::normalize(camera.GetFront());

    for (float t = 0.0f; t < maxDistance; t += stepSize)
    {
        glm::vec3 pos = rayOrigin + rayDir * t;
        glm::ivec3 blockPos = glm::floor(pos);

        if (world.GetBlockType(blockPos.x, blockPos.y, blockPos.z) != BlockType::Air)
        {
            glm::vec3 prevPos = rayOrigin + rayDir * (t - stepSize);
            glm::ivec3 prevBlockPos = glm::floor(prevPos);
            glm::ivec3 normal = glm::clamp(prevBlockPos - blockPos, glm::ivec3(-1), glm::ivec3(1));

            return { true, normal, blockPos };
        }
    }
    return { false, {}, {} };
}
