#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <string.h>

#include "terrain.h"

void BaseTerrain::LoadFromFile(const char* pFilename) {
	int FileSize = 0;
	unsigned char* p = (unsigned char*)ReadBinaryFile(pFilename, FileSize);

	assert(FileSize % sizeof(float) == 0);		//Da se osigurame deka sodrzi cel broj na floats
	m_terrainSize = sqrt(FileSize / sizeof(float));
	m_heihgtMap.InitArray2D(m_terrainSize, m_terrainSize, p);
	m_heihgtMap.PrintFloat();		//Da proverime dali sodrzinata od file e vctina
}



