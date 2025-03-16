#version 330 core

out vec4 o_Color;

in vec2 u_TextureCoordinates;
in vec4 u_TextureColor;

uniform sampler2D u_Texture;

void main()
{
    o_Color = u_TextureColor * texture(u_Texture, u_TextureCoordinates);
} 
