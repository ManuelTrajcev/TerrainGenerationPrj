
#version 330

layout (locatiom = 0) in vec3 Pos0;
layout (locatiom = 1) in vec3 Tex0;

out vec2 Tex1;

void main() 
{
	gl_Position = vec4(Pos0, 1.0);

	Tex1 = Tex0;
}
