
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <string.h>

#include "terrain.h"
#include "texture_config.h"
#include "3rdparty/stb_image_write.h"

//#define DEBUG_PRINT

BaseTerrain::~BaseTerrain()
{
    Destroy();
}


void BaseTerrain::Destroy()
{
    m_heightMap.Destroy();
    m_geomipGrid.Destroy();
}



void BaseTerrain::InitTerrain(float WorldScale, float TextureScale, const std::vector<string>& TextureFilenames)
{
    if (!m_terrainTech.Init()) {
        printf("Error initializing tech\n");
        exit(0);
    }
	
    if (TextureFilenames.size() != ARRAY_SIZE_IN_ELEMENTS(m_pTextures)) {
        printf("%s:%d - number of provided textures (%d) is not equal to the size of the texture array (%d)\n",
               __FILE__, __LINE__, (int)TextureFilenames.size(), (int)ARRAY_SIZE_IN_ELEMENTS(m_pTextures));
        exit(0);
    }

    m_worldScale = WorldScale;
    m_textureScale = TextureScale;

    for (int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_pTextures) ; i++) {
        m_pTextures[i] = new Texture(GL_TEXTURE_2D);
        m_pTextures[i]->Load(TextureFilenames[i]);
    }
}


void BaseTerrain::Finalize()
{
    m_geomipGrid.CreateGeomipGrid(m_terrainSize, m_terrainSize, m_patchSize, this);
}





void BaseTerrain::LoadFromFile(const char* pFilename)
{
    LoadHeightMapFile(pFilename);

    // how do we know the patch size at this point?
    assert(0);

    m_geomipGrid.CreateGeomipGrid(m_terrainSize, m_terrainSize, m_patchSize, this);
}


void BaseTerrain::LoadHeightMapFile(const char* pFilename)
{
    int FileSize = 0;
    unsigned char* p = (unsigned char*)ReadBinaryFile(pFilename, FileSize);

    if (FileSize % sizeof(float) != 0) {
        printf("%s:%d - '%s' does not contain an whole number of floats (size %d)\n", __FILE__, __LINE__, pFilename, FileSize);
        exit(0);
    }

    m_terrainSize = (int)sqrtf((float)FileSize / (float)sizeof(float));

    printf("Terrain size %d\n", m_terrainSize);

    if ((m_terrainSize * m_terrainSize) != (FileSize / sizeof(float))) {
        printf("%s:%d - '%s' does not contain a square height map - size %d\n", __FILE__, __LINE__, pFilename, FileSize);
        exit(0);
    }

    m_heightMap.InitArray2D(m_terrainSize, m_terrainSize, (float*)p);
}


void BaseTerrain::SaveToFile(const char* pFilename)
{    
    unsigned char* p = (unsigned char*)malloc(m_terrainSize * m_terrainSize);

    float* src = m_heightMap.GetBaseAddr();

    float Delta = m_maxHeight - m_minHeight;

    for (int i = 0; i < m_terrainSize * m_terrainSize; i++) {
        float f = (src[i] - m_minHeight) / Delta;
        p[i] = (unsigned char)(f * 255.0f);
    }

    stbi_write_png("heightmap.png", m_terrainSize, m_terrainSize, 1, p, m_terrainSize);

    free(p);
}


void BaseTerrain::Render(const BasicCamera& Camera)
{
    Matrix4f VP = Camera.GetViewProjMatrix();
    Matrix4f View = Camera.GetMatrix();

    m_terrainTech.Enable();
    m_terrainTech.SetVP(VP);

    for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pTextures); i++) {
        if (m_pTextures[i]) {
            m_pTextures[i]->Bind(COLOR_TEXTURE_UNIT_0 + i);
        }
    }
	
    m_terrainTech.SetLightDir(m_lightDir);

    m_geomipGrid.Render(Camera.GetPos(), VP);
}


void BaseTerrain::SetMinMaxHeight(float MinHeight, float MaxHeight)
{
    m_minHeight = MinHeight;
    m_maxHeight = MaxHeight;

    m_terrainTech.Enable();
    m_terrainTech.SetMinMaxHeight(MinHeight, MaxHeight);
}


void BaseTerrain::SetTextureHeights(float Tex0Height, float Tex1Height, float Tex2Height, float Tex3Height)
{
    m_terrainTech.SetTextureHeights(Tex0Height, Tex1Height, Tex2Height, Tex3Height); 
}






