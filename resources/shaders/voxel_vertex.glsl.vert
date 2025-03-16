#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in uint a_Bits;
layout (location = 2) in vec3 a_BlockPos;

uniform mat4 u_Mvp;
uniform vec4 u_Raycast;

out vec2 u_TextureCoordinates;
out vec4 u_TextureColor;

const float tileSize = 16.0 / 256.0;

const vec2 uvs[4] = vec2[4](
    vec2(0.0, 0.0),
    vec2(tileSize, 0.0),
    vec2(tileSize, tileSize),
    vec2(0.0, tileSize)
);

void main()
{
    gl_Position = u_Mvp * vec4(a_Pos.x, a_Pos.y, a_Pos.z, 1.0);

    uint uvType     =  a_Bits        & 0x3u;       // bits 0-1
    uint direction  = (a_Bits >> 2)  & 0x7u;       // bits 2-4
    uint textureX   = (a_Bits >> 5)  & 0xFu;       // bits 5-8
    uint textureY   = (a_Bits >> 9)  & 0xFu;       // bits 9-12

    vec2 textureCoordinates = vec2(tileSize * textureX, 1.0 - tileSize - tileSize * textureY) + uvs[uvType];
    u_TextureCoordinates = textureCoordinates;

    // this is grass, im hardcoding it because im a good programmer
    if (textureX == 0u && textureY == 0u)
    {
        u_TextureColor = vec4(124.0 / 255.0, 189.0 / 255.0, 107.0 / 255.0, 1.0);
    } else {
        float f = 1.0;
        f -= float(direction) * 0.05f;

        // back, bottom, right will have slightly darker color?
        if (direction == 1u || direction == 2u || direction == 3u || direction == 5u) {
            u_TextureColor = vec4(0.7, 0.7, 0.7, 1.0);
        } else {
            u_TextureColor = vec4(1.0, 1.0, 1.0, 1.0);
        }

        u_TextureColor = vec4(f, f, f, 1.0);
    }

    if (u_Raycast.w == 1)
    {
        if (a_BlockPos.x == u_Raycast.x && a_BlockPos.y == u_Raycast.y && a_BlockPos.z == u_Raycast.z)
        {
            u_TextureColor *= 1.5;
        }
    }
}
