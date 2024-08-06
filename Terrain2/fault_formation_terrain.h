#ifndef FAULT_FORMATION_TERRAIN_H
#define FAULT_FORMATION_TERRAIN_H

#include "terrain.h"

class FaultFormationTerrain :public BaseTerrain {
public:
	FaultFormationTerrain() {}

	void CreateFaultFormation(int TerrainSize, int Iterations, float MinHeight, float MaxHeight);
private:
	struct TerainPoint {
		int x = 0;
		int z = 0;
		void Print() {
			printf("[%d,%d]", x, z);
		}

		bool IsEqual(TerainPoint& p) const {
			return ((x == p.x) && (z == p.z));
		}
	};
	void CreateFaultFormationInternal(int Iterations, float MinHeight, float MaxHeight);
	void GetRandomTerrainPoints(TerainPoint& p1, TerainPoint& p2);
};

#endif
