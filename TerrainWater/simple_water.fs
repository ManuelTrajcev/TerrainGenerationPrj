#version 330

layout(location = 0) out vec4 FragColor;

in vec2 oTex;
in vec4 ClipSpaceCoords;
in vec3 oVertexToCamera;     //for calcualting frensel effect

uniform sampler2D gReflectionTexture;
uniform sampler2D gRefractionTexture;
uniform sampler2D gDUDVMapTexture;
uniform float gDUDVOffset = 0.0;        //Move Factor
uniform vec3 gLightColor;
uniform vec3 gReversedLightDir;     //fromLightVector
uniform sampler2D gDepthMap;
uniform sampler2D gNormalMap;

const float WaveLength  = 0.02;
const float Shininess  = 20.0;
const float Reflectivity = 1.6;

void main()
{
    
    vec2 NDCCoords = (ClipSpaceCoords.xy / ClipSpaceCoords.w) / 2.0f + 0.5f;      //Normalized device coordinates (screen cords)
    vec2 RefractionTexCoords = NDCCoords.xy;
    vec2 ReflectionTexCoords = RefractionTexCoords;
    ReflectionTexCoords.y = 1.0 - ReflectionTexCoords.y;
   
    float Depth = texture(gDepthMap, RefractionTexCoords).r;        //number between 0 and 1, r - red component from the texture
    
    //Convert Depth(0-1) to distance
    float ZNear = 1.0;
    float ZFar = 1000.0;
    float FloorDistance = 2.0 * ZNear * ZFar / (ZFar + ZNear - (2.0 * Depth - 1.0) * (ZFar - ZNear));
    float WaterDistance = 2.0 * ZNear * ZFar / (ZFar + ZNear - (2.0 * gl_FragCoord.z - 1.0) * (ZFar - ZNear));
    float FloorToWaterSurface = FloorDistance - WaterDistance;      //Water depth

    vec2 dudv1 = texture(gDUDVMapTexture, vec2(oTex.x + gDUDVOffset, oTex.y)).rg * 0.1;     //Distortion
    dudv1 = oTex + vec2(dudv1.x, dudv1.y + gDUDVOffset);
    vec2 dudv = (texture(gDUDVMapTexture, dudv1).rg * 2.0 - 1) * WaveLength * clamp(FloorToWaterSurface / 20.0, 0.0, 1.0);
    ReflectionTexCoords = clamp(ReflectionTexCoords + dudv, 0.001, 0.999);
    RefractionTexCoords = clamp(RefractionTexCoords + dudv, 0.001, 0.999);
    vec4 reflectionColor = texture(gReflectionTexture, ReflectionTexCoords);
    vec4 refractionColor = texture(gRefractionTexture, RefractionTexCoords);
    
    vec4 NormalColor = texture(gNormalMap, dudv1);      //Normal map
    vec3 Normal = vec3(NormalColor.r * 2.0 - 1.0, NormalColor.b * 4.0, NormalColor.g * 2.0 - 1.0);
    Normal = normalize(Normal);

    vec3 ViewVector = normalize(oVertexToCamera);
    float refractiveFactor = dot(ViewVector, Normal);
    refractiveFactor = pow(refractiveFactor, 2.0);     //higher number - more reflection

    vec3 ReflectedLight = reflect(gReversedLightDir, Normal);
    float SpecularFactor = max(dot(ReflectedLight, ViewVector), 0.0);
    SpecularFactor = pow(SpecularFactor, Shininess);
    vec4 SpecularColor = vec4(gLightColor * SpecularFactor * Reflectivity * clamp(FloorToWaterSurface / 10.0, 0.0, 1.0), 0.0);

    FragColor = mix(reflectionColor, refractionColor, 0.5);
    FragColor = mix(FragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + SpecularColor;   //Mix with blue-green color
    FragColor.a = clamp(FloorToWaterSurface / 10.0, 0.0, 1.0);       //Deeper water -  darker color    

}