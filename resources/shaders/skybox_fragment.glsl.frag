#version 330 core
out vec4 o_Color;

in vec3 o_TextureCoordinates;

uniform samplerCube u_Texture;

void main()
{
    o_Color = texture(u_Texture, o_TextureCoordinates);
} 
