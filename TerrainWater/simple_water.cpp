#include "simple_water.h"
#include "texture_config.h"

SimpleWater::SimpleWater() : m_dudvMap(GL_TEXTURE_2D), m_normalMap(GL_TEXTURE_2D)
{
}


SimpleWater::~SimpleWater()
{
}

void SimpleWater::Render(const Vector3f& CameraPos, const Matrix4f& VP, const Vector3f& LightDir)
{
    m_waterTech.Enable();
    m_waterTech.SetVP(VP);
    m_waterTech.SetCameraPos(CameraPos);
    m_waterTech.SetWaterHeight(m_waterHeight);
    m_waterTech.SetLightDir(LightDir);

    static float sTime = 0.0f;
    sTime += 0.001f;
    m_waterTech.SetDUDVOffset(sinf(sTime));

    m_reflectionFBO.BindForReading(REFLECTION_TEXTURE_UNIT);

    m_refractionFBO.BindForReading(REFRACTION_TEXTURE_UNIT);

    m_refractionFBO.BindDepthForReading(DEPTH_MAP_TEXTURE_UNIT);

    m_dudvMap.Bind(DUDV_TEXTURE_UNIT);
    m_normalMap.Bind(NORMAL_MAP_TEXTURE_UNIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_water.Render();
    glDisable(GL_BLEND);
}