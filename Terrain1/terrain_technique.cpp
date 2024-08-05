#include "ogldev_util.h"
#include "terrain_technique.h"


TerrainTechnique::TerrainTechnique()
{
}

bool TerrainTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "terrain.vs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "terrain.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_VPLoc = GetUniformLocation("gVP");

    if (m_VPLoc == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return true;
}


void TerrainTechnique::SetVP(const Matrix4f& VP)
{
    glUniformMatrix4fv(m_VPLoc, 1, GL_TRUE, (const GLfloat*)VP.m);
}
