#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in uint a_Bits;
layout (location = 2) in vec3 a_BlockPos;

uniform float u_Time;
uniform mat4 u_Mvp;
uniform vec4 u_Raycast;

out vec2 u_TextureCoordinates;
out vec4 u_TextureColor;
out vec3 u_Normal;

const float tileSize = 16.0 / 256.0;
const float lightMultiplier = 0.05;
const float darkestLight = 0.1;

const vec2 uvs[4] = vec2[4](
    vec2(0.0, 0.0),
    vec2(tileSize, 0.0),
    vec2(tileSize, tileSize),
    vec2(0.0, tileSize)
);

const vec3 normals[6] = vec3[](
    vec3(0.0, 0.0, 1.0),   // Front
    vec3(0.0, 0.0, -1.0),  // Back
    vec3(0.0, 1.0, 0.0),   // Top
    vec3(0.0, -1.0, 0.0),  // Bottom
    vec3(-1.0, 0.0, 0.0),  // Left
    vec3(1.0, 0.0, 0.0)    // Right
);

void main()
{
    uint uvType     =  a_Bits        & 0x3u;       // bits 0-1 (2 bits)
    uint direction  = (a_Bits >> 2u) & 0x7u;       // bits 2-4 (3 bits)
    uint textureX   = (a_Bits >> 5u) & 0xFu;       // bits 5-8 (4 bits)
    uint textureY   = (a_Bits >> 9u) & 0xFu;       // bits 9-12 (4 bits)
    uint skyLight   = (a_Bits >> 13u) & 0xFu;      // bits 13-16 (4 bits)
    uint blockLight = (a_Bits >> 17u) & 0xFu;      // bits 17-20 (4 bits)

    vec2 textureCoordinates = vec2(tileSize * textureX, 1.0 - tileSize - tileSize * textureY) + uvs[uvType];

    u_TextureColor = vec4(1.0, 1.0, 1.0, 1.0);

    // this is grass, im hardcoding it because im a good programmer
    if (textureX == 0u && textureY == 0u)
    {
        u_TextureColor = vec4(124.0 / 255.0, 189.0 / 255.0, 107.0 / 255.0, 1.0);
    }
    else
    {
        // bottom, left, right will have slightly darker color?
        if (direction == 3u || direction == 4u || direction == 5u)
        {
            u_TextureColor = vec4(0.7, 0.7, 0.7, 1.0);
        }
    }

    u_Normal = normals[direction];
    gl_Position = u_Mvp * vec4(a_Pos.x, a_Pos.y, a_Pos.z, 1.0);
    u_TextureCoordinates = textureCoordinates;

    if (textureX == 13u && textureY == 12u)
    {
        float crazyFactor = 0.2;
        float freq = 1.0;
        float timeSpeed = u_Time * 1.0;

        gl_Position.y += sin(gl_Position.x * freq + timeSpeed) * crazyFactor;
        gl_Position.y += cos(gl_Position.z * freq + timeSpeed) * crazyFactor;

        u_TextureColor.a = 0.5;
    }

    if (u_Raycast.w == 1 && a_BlockPos.x == u_Raycast.x && a_BlockPos.y == u_Raycast.y && a_BlockPos.z == u_Raycast.z)
    {
        u_TextureColor *= 1.5;
    }
}
