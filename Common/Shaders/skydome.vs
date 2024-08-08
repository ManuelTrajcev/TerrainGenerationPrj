#version 410

layout (location = 0) in vec3 Position;

uniform mat4 gWVP = mat4(1.0);

out float Height;

void main()
{
    gl_Position = gWVP * vec4(Position, 1.0);

    gl_Position.z = gl_Position.w;

    Height = Position.y;

}
