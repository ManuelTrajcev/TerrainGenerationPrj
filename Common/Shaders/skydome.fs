#version 410

layout(location = 0) out vec4 FragColor;


uniform vec4 gLowColor = vec4(253.0/256.0, 94.0/256.0, 83.0/256.0, 1.0);
uniform vec4 gHighColor = vec4(21.0/256.0, 40.0/256.0, 82.0/256.0, 1.0);

in float Height;

void main()
{
      FragColor = mix(gLowColor, gHighColor, Height);
}
