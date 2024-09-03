#ifndef MIDPOINT_DISP_TERRAIN_H
#define MIDPOINT_DISP_TERRAIN_H

#include "terrain.h"

class MidpointDispTerrain : public BaseTerrain {

 public:
    MidpointDispTerrain() {}

    void CreateMidpointDisplacement(int Size, int PatchSize, float Roughness, float MinHeight, float MaxHeight);

 private:
    void CreateMidpointDisplacementF32(float Roughness);
    void DiamondStep(int RectSize, float CurHeight);
    void SquareStep(int RectSize, float CurHeight);
    float Falloff(float x, float y, float maxDistance);
    void SmoothHeightMap(float threshold, bool isFirst);
    float RadialFalloff(float x, float y, float size, float minHeight, float maxHeight, float waterHeight);
    void ApplyIslandFalloff(float scale);
    void AdjustOutterHeightMap(float waterHeight);
    float m_maxHeight;
};

#endif
