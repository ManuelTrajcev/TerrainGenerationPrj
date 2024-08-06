#version 330

layout(location = 0) out vec4 FragColor;

in vec4 Color;
in vec2 Tex;

uniform sampler2D gTerrainTexture;

void main()
{
    vec4 TexColor = texture(gTerrainTexture, Tex);      //Sample from the texture
    FragColor = Color * TexColor;       //Higher places - brighter
}