
#ifndef SIMPLE_WATER_TECHNIQUE_H
#define SIMPLE_WATER_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"

class SimpleWaterTechnique : public Technique
{
public:

    SimpleWaterTechnique();

    virtual bool Init();

    void SetVP(const Matrix4f& VP);
    void SetReflectionTextureUnit(unsigned int TextureUnit);
    void SetRefractionTextureUnit(unsigned int TextureUnit);
    void SetDUDVMapTextureUnit(unsigned int TextureUnit);
    void SetNormalMapTextureUnit(unsigned int TextureUnit);
    void SetDepthMapTextureUnit(unsigned int TextureUnit);
    void SetWaterHeight(float Height);
    void SetDUDVOffset(float Offset);
    void SetCameraPos(const Vector3f& CameraPos);
    void SetLightColor(const Vector3f& LightColor);
    void SetLightDir(const Vector3f& LightDir);

private:
    GLuint m_VPLoc = -1;
    GLuint m_reflectionTexUnitLoc = -1;
    GLuint m_refractionTexUnitLoc = -1;
    GLuint m_heightLoc = -1;
    GLuint m_dudvMapTexUnitLoc = -1;
    GLuint m_normalMapTexUnitLoc = -1;
    GLuint m_depthMapTexUnitLoc = -1;
    GLuint m_dudvOffsetLoc = -1;
    GLuint m_cameraPosLoc = -1;    
    GLuint m_lightColorLoc = -1;
    GLuint m_reversedLightDirLoc = -1;
};

#endif  /* SIMPLE_WATER_TECHNIQUE_H */
