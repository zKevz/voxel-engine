#version 330 core
layout (location = 0) in vec3 a_Pos;

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec3 o_TextureCoordinates;

void main()
{
    vec4 pos = u_Projection * u_View * vec4(a_Pos, 1.0);
    gl_Position = pos.xyww;
    o_TextureCoordinates = a_Pos;
}
