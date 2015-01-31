/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <limits.h>
#include <string.h>

#include "ogldev_math_3d.h"
#include "ogldev_util.h"
#include "ssao_technique.h"


SSAOTechnique::SSAOTechnique()
{   
}


bool SSAOTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "shaders/ssao.vs")) {
        return false;
    }


    if (!AddShader(GL_FRAGMENT_SHADER, "shaders/ssao.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

	m_posTextureUnitLocation = GetUniformLocation("gPositionMap");
	//m_normalTextureUnitLocation = GetUniformLocation("gNormalMap");
    m_randomTextureUnitLocation = GetUniformLocation("gRandomMap");
    m_depthTextureUnitLocation = GetUniformLocation("gDepthMap");
    m_screenSizeLocation = GetUniformLocation("gScreenSize");	
    m_AILocation = GetUniformLocation("gIntensity");
    m_sampleRadLocation = GetUniformLocation("gSampleRad");
    m_zNearLocation = GetUniformLocation("gzNear");
    m_zFarLocation = GetUniformLocation("gzFar");
    m_projMatrixLocation = GetUniformLocation("gProj");	
    m_kernelLocation = GetUniformLocation("gKernel");

    if (m_posTextureUnitLocation    == INVALID_UNIFORM_LOCATION ||
		m_normalTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
        m_randomTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
        m_depthTextureUnitLocation  == INVALID_UNIFORM_LOCATION ||
        m_screenSizeLocation        == INVALID_UNIFORM_LOCATION ||
        m_AILocation                == INVALID_UNIFORM_LOCATION ||
        m_sampleRadLocation         == INVALID_UNIFORM_LOCATION ||
        m_zNearLocation             == INVALID_UNIFORM_LOCATION ||
        m_zFarLocation              == INVALID_UNIFORM_LOCATION ||
        m_projMatrixLocation        == INVALID_UNIFORM_LOCATION ||		
        m_kernelLocation            == INVALID_UNIFORM_LOCATION) {
    //    return false;
    }
   
    Enable();
    
    GLExitIfError;
    
    GenKernel();
    
    GLExitIfError;
    
    return GLCheckError();
}


void SSAOTechnique::GenKernel()
{
    Vector3f kernel[KERNEL_SIZE];
    
    for (uint i = 0 ; i < KERNEL_SIZE ; i++ ) {
        float scale = (float)i / (float)(KERNEL_SIZE);        
        Vector3f v;
        v.x = 2.0f * (float)rand()/RAND_MAX - 1.0f;
        v.y = 2.0f * (float)rand()/RAND_MAX - 1.0f;
        v.z = (float)rand()/RAND_MAX;
        v *= (0.1f + 0.9f * scale * scale);
        
        v.Print();
        printf("\n");
        kernel[i] = v;
        //glUniform3f(m_kernelLocation[i], v.x, v.y, v.z);
    }
       
    glUniform3fv(m_kernelLocation, KERNEL_SIZE, (const GLfloat*)&kernel[0]);    
}


void SSAOTechnique::SetPositionTextureUnit(uint TextureUnit)
{
    glUniform1i(m_posTextureUnitLocation, TextureUnit);
}


void SSAOTechnique::SetNormalTextureUnit(uint TextureUnit)
{
    glUniform1i(m_normalTextureUnitLocation, TextureUnit);
}


void SSAOTechnique::SetRandomTextureUnit(uint TextureUnit)
{
    glUniform1i(m_randomTextureUnitLocation, TextureUnit);
}


void SSAOTechnique::SetDepthTextureUnit(uint TextureUnit)
{
    glUniform1i(m_depthTextureUnitLocation, TextureUnit);
}


void SSAOTechnique::SetScreenSize(uint Width, uint Height)
{
    glUniform2f(m_screenSizeLocation, (float)Width, (float)Height);
}


void SSAOTechnique::SetAmbientIntensity(float ai)
{
    glUniform1f(m_AILocation, ai);
}


void SSAOTechnique::SetSampleRadius(float sr)
{
    glUniform1f(m_sampleRadLocation, sr);
}


void SSAOTechnique::SetProjMatrix(const Matrix4f& m)
{
    glUniformMatrix4fv(m_projMatrixLocation, 1, GL_TRUE, (const GLfloat*)m.m);    
}


void SSAOTechnique::SetZNearAndFar(float zNear, float zFar)
{
    glUniform1f(m_zNearLocation, zNear);
    glUniform1f(m_zFarLocation, zFar);
}
