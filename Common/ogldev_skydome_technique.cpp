#include "ogldev_skydome_technique.h"


SkydomeTechnique::SkydomeTechnique()
{
}

bool SkydomeTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "../Common/Shaders/skydome.vs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "../Common/Shaders/skydome.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_WVPLoc = GetUniformLocation("gWVP");
    m_samplerLoc = GetUniformLocation("gSampler");

    if (m_WVPLoc == INVALID_UNIFORM_LOCATION ||
        m_samplerLoc == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return true;
}


void SkydomeTechnique::SetTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_samplerLoc, TextureUnit);
}


void SkydomeTechnique::SetWVP(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_WVPLoc, 1, GL_TRUE, (const GLfloat*)WVP.m);
}
