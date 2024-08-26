#include "simple_water.h"
#include "texture_config.h"

SimpleWater::SimpleWater() : m_dudvMap(GL_TEXTURE_2D), m_normalMap(GL_TEXTURE_2D)
{
}


SimpleWater::~SimpleWater()
{
}


void SimpleWater::Init(int Size, float WorldScale)
{
    if (!m_waterTech.Init()) {
        printf("Error initializing water tech\n");
        exit(0);
    }

    m_waterTech.Enable();
    m_waterTech.SetReflectionTextureUnit(REFLECTION_TEXTURE_UNIT_INDEX);
    m_waterTech.SetRefractionTextureUnit(REFRACTION_TEXTURE_UNIT_INDEX);
    m_waterTech.SetDUDVMapTextureUnit(DUDV_TEXTURE_UNIT_INDEX);
    m_waterTech.SetNormalMapTextureUnit(NORMAL_MAP_TEXTURE_UNIT_INDEX);
    m_waterTech.SetDepthMapTextureUnit(DEPTH_MAP_TEXTURE_UNIT_INDEX);
    m_waterTech.SetWaterHeight(m_waterHeight);
    m_waterTech.SetLightColor(Vector3f(1.0f, 1.0f, 1.0f));

    m_dudvMap.Load("../Content/waterDUDV.png");
    m_normalMap.Load("../Content/WaterNormalMap.png");

    m_water.CreateTriangleList(2, 2, Size * WorldScale);

    m_reflectionFBO.Init(1000, 1000);
    m_refractionFBO.Init(1000, 1000);
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


void SimpleWater::StartReflectionPass()
{
    m_reflectionFBO.BindForWriting();
}


void SimpleWater::EndReflectionPass()
{
    m_reflectionFBO.UnbindWriting();
}


void SimpleWater::StartRefractionPass()
{
    m_refractionFBO.BindForWriting();
}


void SimpleWater::EndRefractionPass()
{
    m_refractionFBO.UnbindWriting();
}