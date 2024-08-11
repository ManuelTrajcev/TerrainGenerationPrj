#ifndef SIMPLE_WATER_H
#define SIMPLE_WATER_H

#include "ogldev_texture.h"
#include "ogldev_framebuffer.h"
#include "simple_water_technique.h"
#include "triangle_list.h"

class SimpleWater {
private:
    TriangleList m_water;
    SimpleWaterTechnique m_waterTech;
    float m_waterHeight = 0.0f;
    Framebuffer m_reflectionFBO;
    Framebuffer m_refractionFBO;
    Texture m_dudvMap;
    Texture m_normalMap;

public:
	SimpleWater();
	~SimpleWater();

	void Init(int Size, float WorldScale);
    void SetWaterHeight(float Height) { m_waterHeight = Height; }
    float GetWaterHeight() const { return m_waterHeight; }
    void Render(const Vector3f& CameraPos, const Matrix4f& VP, const Vector3f& LightDir);

    void StartReflectionPass();
    void EndReflectionPass();
    void StartRefractionPass();
    void EndRefractionPass();

    GLuint GetReflectionTexture() const { return m_reflectionFBO.GetTexture(); }
    GLuint GetRefractionTexture() const { return m_refractionFBO.GetTexture(); }
};




#endif
