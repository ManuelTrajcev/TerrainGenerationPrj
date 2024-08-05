#ifndef TERRAIN_H
#define TERRAIN_H

#include "ogldev_types.h"
#include "ogldev_basic_glfw_camera.h"
#include "ogldev_array_2d.h"
#include "triangle_list.h"
#include "terrain_technique.h"


class BaseTerrain 
{
public:
	BaseTerrain(){}

	void LoadFromFile(const char* pFilename);

protected:
	int m_terrainSize = 0;
	Array2D<float> m_heihgtMap;
};

#endif
