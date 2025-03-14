#version 330 core

out vec4 o_Color;

in vec2 u_TextureCoordinates;
in vec4 u_TextureColor;

uniform sampler2D u_Texture;

void main()
{
    if (u_TextureColor.x != 0) {
        o_Color = u_TextureColor * texture(u_Texture, u_TextureCoordinates);
    } else {
        o_Color = texture(u_Texture, u_TextureCoordinates);
    }
} 
