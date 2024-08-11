#version 330

layout(location = 0) out vec4 FragColor;

in vec2 oTex;
in vec4 ClipSpaceCoords;
in vec3 oVertexToCamera;

uniform sampler2D gReflectionTexture;
uniform sampler2D gRefractionTexture;
uniform sampler2D gDUDVMapTexture;

const float waveStrength = 0.02;

void main()
{
    
    vec4 NDCCoords = (ClipSpaceCoords / ClipSpaceCoords.w) / 2.0f + vec4(0.5);      //Normalized device coordinates (screen cords
    vec2 RefractionTexCoords = NDCCoords.xy;
    vec2 ReflectionTexCoords = RefractionTexCoords;
    ReflectionTexCoords.y = 1.0 - ReflectionTexCoords.y;

    vec2 distortion1 = (texture(gDUDVMapTexture, vec2(oTex.x, oTex.y)).rg * 2.0 + 1.0) * waveStrength;

    RefractionTexCoords += distortion1;

    vec4 reflectionColor = texture(gReflectionTexture, ReflectionTexCoords + vec2(-0.1, 0.0);
    vec4 reflectionColor = texture(gRefractionTexture, RefractionTexCoords);

    RefractionTexCoords = clamp(RefractionTexCoords, 0.001, 0.999);

    ReflectionTexCoords = clamp(ReflectionTexCoords.x, 0.001, 0.999);
    ReflectionTexCoords = clamp(ReflectionTexCoords.y, -0.99, -0.001);
    

    FragColor = mix(reflectionColor, reflectionColor, 0.5);
}