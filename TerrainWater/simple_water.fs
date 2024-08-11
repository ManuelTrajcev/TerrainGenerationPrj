#version 330

layout(location = 0) out vec4 FragColor;

in vec2 oTex;
in vec4 ClipSpaceCoords;
in vec3 oVertexToCamera;





void main()
{
    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}