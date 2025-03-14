#version 400 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in uint a_Bits;

uniform mat4 u_Mvp;

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

    if (textureX == 0 && textureY == 0) {
        u_TextureColor = vec4(124.0 / 255.0, 189.0 / 255.0, 107.0 / 255.0, 1.0);
    } else {
        u_TextureColor = vec4(0.0);
    }
}
