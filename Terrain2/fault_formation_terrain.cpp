#include "fault_formation_terrain.h"

void FaultFormationTerrain::CreateFaultFormation(int TerrainSize, int Iterations, float MinHeight, float MaxHeight) {
	m_terrainSize = TerrainSize;
	m_minHeight = MinHeight;
	m_maxHeight = MaxHeight;

	m_heightMap.InitArray2D(m_terrainSize, m_terrainSize, this);
	CreateFaultFormationInternal(Iterations, MinHeight, MaxHeight);
	m_heightMap.Normalize(MinHeight, MaxHeight);
	m_triangleList.CreateTriangleList(m_terrainSize, m_terrainSize, this);

}

void FaultFormationTerrain::CreateFaultFormationInternal(int Iterations, float MinHeight, float MaxHeight) {
	float DeltaHeight = MaxHeight - MinHeight;

	for (int CurIter = 0; CurIter < Iterations; Iterations++)
	{
		float IterationRatio = ((float)CurIter / (float)Iterations);
		float Height = MaxHeight - IterationRatio * DeltaHeight;		//Decreese in iterations

		TerainPoint p1, p2;
		GetRandomTerrainPoints(p1, p2);

		int DirX = p2.x - p1.x;		//Pravecot X e delta na x-oska
		int DirZ = p2.z - p1.z;		//Pravecot Z e delta na z-oska

		for (int z = 0; z < m_terrainSize; z++) {
			for (int x = 0; x < m_terrainSize; x++) {
				int DirX_in = x - p1.x;
				int DirZ_in = z - p1.z;

				int CrossProduct = DirX_in * DirZ - DirX * DirZ_in;		//CrossProduct=Vektor normalen na ramninata na dva vektora

				if (CrossProduct > 0) {		//Se zgolemuva visinata na tockata na koja se naogja CrossProduct vektorot
					float CurHeight = m_heightMap.Get(x, z);		
					m_heightMap.Set(x, z, CurHeight + Height);		//Problem, ako izleze od MaxHeight --> m_heightNao.Normalize() 
				}
			}
		}
	}
}

void FaultFormationTerrain::GetRandomTerrainPoints(TerainPoint& p1, TerainPoint& p2) {		//2 random terrain points vo range na terrain size
	p1.x = rand() % m_terrainSize;
	p1.z = rand() % m_terrainSize;

	int Counter = 0;

	do {		//Za da ne se dobijat dve isti tocki
		p2.x = rand() % m_terrainSize;
		p2.z = rand() % m_terrainSize;

		if (Counter++ == 1000) {
			printf("Endless loop detected in %s:%d\n", __FILE__, __LINE__);
			assert(0);
		}
	} while (p1.IsEqual(p2));

}