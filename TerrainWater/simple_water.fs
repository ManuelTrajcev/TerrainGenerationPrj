#version 330

layout(location = 0) out vec4 FragColor;

in vec2 oTex;
in vec4 ClipSpaceCoords;
in vec3 oVertexToCamera;

uniform sampler2D gReflectionTexture;
uniform sampler2D gRefractionTexture;



void main()
{
    vec4 reflectionColor = texture(gReflectionTexture, ReflectionTexCoords);
    vec4 reflectionColor = texture(gRefractionTexture, RefractionTexCoords);

    vec4 NDCCoords = (ClipSpaceCoords / ClipSpaceCoords.w) / 2.0f + vec4(0.5);      //Normalized device coordinates (screen cords
    vec2 RefractionTexCoords = NDCCoords.xy;
    vec2 ReflectionTexCoords = RefractionTexCoords;
    ReflectionTexCoords.y = 1.0 - ReflectionTexCoords.y;

    FragColor = mix(reflectionColor, reflectionColor, 0.5);
}