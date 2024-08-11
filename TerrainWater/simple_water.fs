#version 330

layout(location = 0) out vec4 FragColor;

in vec2 oTex;
in vec4 ClipSpaceCoords;
in vec3 oVertexToCamera;

uniform sampler2D gReflectionTexture;
uniform sampler2D gRefractionTexture;
uniform sampler2D gDUDVMapTexture;
uniform float gDUDVOffset = 0.0;        //Move Factor

const float waveStrength = 0.02;

void main()
{
    
    vec4 NDCCoords = (ClipSpaceCoords / ClipSpaceCoords.w) / 2.0f + vec4(0.5);      //Normalized device coordinates (screen cords
    vec2 RefractionTexCoords = NDCCoords.xy;
    vec2 ReflectionTexCoords = RefractionTexCoords;
    ReflectionTexCoords.y = 1.0 - ReflectionTexCoords.y;

    vec2 dudv1 = texture(gDUDVMapTexture, vec2(oTex.x + gDUDVOffset, oTex.y)).rg * 0.1;
    dudv1 = oTex + vec2(dudv1.x, dudv1.y + gDUDVOffset);
    vec2 dudv = (texture(gDUDVMapTexture, dudv1).rg * 2.0 - 1) * WaveLength * clamp(FloorToWaterSurface / 20.0, 0.0, 1.0);
    RefractionTexCoords += distortion1;

    vec2 dudv1 = texture(gDUDVMapTexture, vec2(oTex.x + gDUDVOffset, oTex.y)).rg * 0.1;     //Distortion
    dudv1 = oTex + vec2(dudv1.x, dudv1.y + gDUDVOffset);
    vec2 dudv = (texture(gDUDVMapTexture, dudv1).rg * 2.0 - 1) * WaveLength * clamp(FloorToWaterSurface / 20.0, 0.0, 1.0);
    ReflectionTexCoords = clamp(ReflectionTexCoords + dudv, 0.001, 0.999);
    RefractionTexCoords = clamp(RefractionTexCoords + dudv, 0.001, 0.999);
    vec4 reflectionColor = texture(gReflectionTexture, ReflectionTexCoords);
    vec4 refractionColor = texture(gRefractionTexture, RefractionTexCoords);
    

    FragColor = mix(reflectionColor, reflectionColor, 0.5);
    FragColor = mix(FragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2);      //Mix with blue-green color
}