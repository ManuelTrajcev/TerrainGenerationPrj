#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <string.h>

#include "terrain.h"

void BaseTerrain::InitTerrain(float WorldScale) {
	m_terrainTech.Init();		//Compile & link Shaders
	m_worldScale = WorldScale;
}

void BaseTerrain::LoadFromFile(const char* pFilename) {		
	LoadHeightMapFile(pFilename);
	m_triangleList.CreateTriangleList(m_terrainSize, m_terrainSize, this);
}

void BaseTerrain::LoadHeightMapFile(const char* pFilename) {		//Citanje od file
	int FileSize = 0;
	unsigned char* p = (unsigned char*)ReadBinaryFile(pFilename, FileSize);

	assert(FileSize % sizeof(float) == 0);		//Da se osigurame deka sodrzi cel broj na floats
	m_terrainSize = sqrt(FileSize / sizeof(float));
	m_heightMap.InitArray2D(m_terrainSize, m_terrainSize, p);
	//m_heihgtMap.PrintFloat();		//Da proverime dali sodrzinata od file e vctina
}

void BaseTerrain :: Render(const BasicCamera& Camera) {
	Matrix4f VP = Camera.GetViewProjMatrix();

	m_terrainTech.Enable();
	m_terrainTech.SetVP(VP);

	m_triangleList.Render();
}



