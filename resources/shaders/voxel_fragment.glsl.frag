#version 330 core

out vec4 o_Color;

in vec2 u_TextureCoordinates;
in vec4 u_TextureColor;
in vec3 u_Normal;

uniform sampler2D u_Texture;
uniform vec3 u_LightDirection;
uniform float u_Ambient;

void main()
{
    vec3 normal = normalize(u_Normal);
    vec3 lightDirection = normalize(u_LightDirection);

    float diffuse = max(dot(normal, lightDirection), 0.0);
    float lighting = clamp(u_Ambient + diffuse, 0.0, 1.0);

    vec4 textureColor = texture(u_Texture, u_TextureCoordinates) * u_TextureColor;
    if (textureColor.a == 0)
    {
        discard;
    }

    o_Color = textureColor * vec4(vec3(lighting), 1.0);
}
