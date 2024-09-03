#include "midpoint_disp_terrain.h"
#include <cmath>
#include <algorithm>

void MidpointDispTerrain::CreateMidpointDisplacement(int TerrainSize, int PatchSize, float Roughness, float MinHeight, float MaxHeight)
{
	m_terrainSize = TerrainSize;
	m_patchSize = PatchSize;
	m_maxHeight = MaxHeight;
	float scale = 1.0;

	SetMinMaxHeight(MinHeight, MaxHeight);

	m_heightMap.InitArray2D(TerrainSize, TerrainSize, 0.0f);

	CreateMidpointDisplacementF32(Roughness);

	for (int i = 0; i < 20; i++)		//smoothening interations
	{
		SmoothHeightMap(0.005, true);		//smoothening factor
	}

	ApplyIslandFalloff(scale);

	m_heightMap.Normalize(MinHeight, MaxHeight);
	Finalize();
}


void MidpointDispTerrain::CreateMidpointDisplacementF32(float Roughness)
{
	int RectSize = CalcNextPowerOfTwo(m_terrainSize);
	float CurHeight = (float)RectSize / 2.0f;
	float HeightReduce = pow(2.0f, -Roughness * 0.7f); // Reduce roughness impact


	while (RectSize > 0) {

		DiamondStep(RectSize, CurHeight);

		SquareStep(RectSize, CurHeight);

		RectSize /= 2;
		CurHeight *= HeightReduce;
	}
}


float MidpointDispTerrain::Falloff(float x, float y, float maxDistance) {
	float center_x = m_terrainSize / 2.0f;
	float center_y = m_terrainSize / 2.0f;
	float k = 0.6f;
	float threshold = 0.3f;

	float distance = sqrt((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y));
	float normalizedDistance = distance / maxDistance;
	float falloff;
	// Quadratic plateau falloff
	if (normalizedDistance <= threshold) {
		falloff = 1.0f;
	}
	else {
		float adjustedDistance = (normalizedDistance - threshold) / (1.0f - threshold);
		falloff = 1.0f - adjustedDistance * adjustedDistance;
	}

	//falloff = std::max(falloff, 0.0f);
	return falloff;

	// Adjusted quadratic falloff
	//return 1.0f - k * (normalizedDistance * normalizedDistance);
}

void MidpointDispTerrain::DiamondStep(int RectSize, float CurHeight)
{
	int HalfRectSize = RectSize / 2;
	float maxDistance = sqrt(2.0f) * m_terrainSize / 2.0f; // Maximum distance from  center

	for (int y = 10; y < m_terrainSize - 10; y += RectSize) {
		for (int x = 10; x < m_terrainSize - 10; x += RectSize) {
			int next_x = (x + RectSize) % m_terrainSize;
			int next_y = (y + RectSize) % m_terrainSize;

			if (next_x < x) {
				next_x = m_terrainSize - 1;
			}

			if (next_y < y) {
				next_y = m_terrainSize - 1;
			}

			float TopLeft = m_heightMap.Get(x, y);
			float TopRight = m_heightMap.Get(next_x, y);
			float BottomLeft = m_heightMap.Get(x, next_y);
			float BottomRight = m_heightMap.Get(next_x, next_y);

			int mid_x = (x + HalfRectSize) % m_terrainSize;
			int mid_y = (y + HalfRectSize) % m_terrainSize;

			float RandValue = RandomFloatRange(CurHeight, -CurHeight);
			float MidPoint = (TopLeft + TopRight + BottomLeft + BottomRight) / 4.0f;

			//falloff
			float falloff = Falloff(mid_x, mid_y, maxDistance);
			float value = (MidPoint + RandValue) * falloff;
			value = std::min(value, m_maxHeight);
			m_heightMap.Set(mid_x, mid_y, value);
		}
	}
}

void MidpointDispTerrain::SquareStep(int RectSize, float CurHeight)
{
	int HalfRectSize = RectSize / 2;
	float maxDistance = sqrt(2.0f) * m_terrainSize / 2.0f; // Max distance from the center

	for (int y = 10; y < m_terrainSize - 10; y += RectSize) {
		for (int x = 10; x < m_terrainSize - 10; x += RectSize) {
			int next_x = (x + RectSize) % m_terrainSize;
			int next_y = (y + RectSize) % m_terrainSize;

			if (next_x < x) {
				next_x = m_terrainSize - 1;
			}

			if (next_y < y) {
				next_y = m_terrainSize - 1;
			}

			int mid_x = (x + HalfRectSize) % m_terrainSize;
			int mid_y = (y + HalfRectSize) % m_terrainSize;

			int prev_mid_x = (x - HalfRectSize + m_terrainSize) % m_terrainSize;
			int prev_mid_y = (y - HalfRectSize + m_terrainSize) % m_terrainSize;

			float CurTopLeft = m_heightMap.Get(x, y);
			float CurTopRight = m_heightMap.Get(next_x, y);
			float CurCenter = m_heightMap.Get(mid_x, mid_y);
			float PrevYCenter = m_heightMap.Get(mid_x, prev_mid_y);
			float CurBotLeft = m_heightMap.Get(x, next_y);
			float PrevXCenter = m_heightMap.Get(prev_mid_x, mid_y);

			float CurLeftMid = (CurTopLeft + CurCenter + CurBotLeft + PrevXCenter) / 4.0f + RandomFloatRange(-CurHeight, CurHeight);
			float CurTopMid = (CurTopLeft + CurCenter + CurTopRight + PrevYCenter) / 4.0f + RandomFloatRange(-CurHeight, CurHeight);

			//falloff
			float falloffTop = Falloff(mid_x, y, maxDistance);
			float falloffLeft = Falloff(x, mid_y, maxDistance);

			float valueTop = CurTopMid * falloffTop;
			valueTop = std::min(valueTop, m_maxHeight);

			float valueLeft = CurLeftMid * falloffLeft;
			valueLeft = std::min(valueLeft, m_maxHeight);

			m_heightMap.Set(mid_x, y, valueTop);
			m_heightMap.Set(x, mid_y, valueLeft);
		}
	}
}


void MidpointDispTerrain::SmoothHeightMap(float threshold, bool isFirst) {
	Array2D<float> tempMap = m_heightMap;
	int borderSize = m_terrainSize / 8;

	if (isFirst) {
		for (int i = 0; i < m_terrainSize; i++)
		{
			tempMap.Set(i, 0, 0.0);
			m_heightMap.Set(i, 0, 0.0);
			tempMap.Set(i, m_terrainSize - 1, 0.0);
			m_heightMap.Set(i, m_terrainSize - 1, 0.0);

			tempMap.Set(0, i, 0.0);
			m_heightMap.Set(0, i, 0.0);

			tempMap.Set(m_terrainSize - 1, i, 0.0);
			m_heightMap.Set(m_terrainSize - 1, i, 0.0);
		}
	}


	for (int y = 1; y < m_terrainSize - 1; ++y) {
		for (int x = 1; x < m_terrainSize - 1; ++x) {
			float currentValue = m_heightMap.Get(x, y);

			// values of all eight neighbors
			float left = m_heightMap.Get(x - 1, y);
			float right = m_heightMap.Get(x + 1, y);
			float top = m_heightMap.Get(x, y - 1);
			float bottom = m_heightMap.Get(x, y + 1);
			float topLeft = m_heightMap.Get(x - 1, y - 1);
			float topRight = m_heightMap.Get(x + 1, y - 1);
			float bottomLeft = m_heightMap.Get(x - 1, y + 1);
			float bottomRight = m_heightMap.Get(x + 1, y + 1);
			float newValue;

		
			float maxDiff = std::max({ std::abs(currentValue - left),
									   std::abs(currentValue - right),
									   std::abs(currentValue - top),
									   std::abs(currentValue - bottom),
									   std::abs(currentValue - topLeft),
									   std::abs(currentValue - topRight),
									   std::abs(currentValue - bottomLeft),
									   std::abs(currentValue - bottomRight) });

			float neighborAvg = (left + right + top + bottom + topLeft + topRight + bottomLeft + bottomRight) / 8.0f;

			if (maxDiff > threshold) {
				float smoothFactor = 0.9f; // smoothing factor for higher points
				// increase smoothing factor for lower points
				if (currentValue > neighborAvg) {
					smoothFactor = 1.0f;
				}

				if (currentValue < m_maxHeight * 0.5)
				{
					newValue = neighborAvg;
				}
				else
				{
					newValue = smoothFactor * neighborAvg + (1.0f - smoothFactor) * currentValue;
				}
				//Option 2
				//newValue = neighborAvg
			//	if (isFirst)
				{
					//	if (x < borderSize || x >= m_terrainSize - borderSize || y < borderSize || y >= m_terrainSize - borderSize) {
					//		if (newValue > GetWaterHeight()) {
					//			float rand = RandomFloatRange(0.6,0.8);
					//			newValue = GetWaterHeight()*rand;
					//		}
					//	}
					//}

					tempMap.Set(x, y, newValue);
				}
			}
		}
		m_heightMap = tempMap;
	}
}

float smoothstep(float edge0, float edge1, float x) {
	float t = (x - edge0) / (edge1 - edge0);
	t = std::max(t, 0.0f);
	t = std::min(t, 1.0f);
	return t * t * (3 - 2 * t);
}


float MidpointDispTerrain::RadialFalloff(float x, float y, float size, float minHeight, float maxHeight, float waterHeight) {
	float nx = (x / size) * 2 - 1;  // Normalize x to range [-1, 1]
	float ny = (y / size) * 2 - 1;  // Normalize y to range [-1, 1]

	float distance = sqrt(nx * nx + ny * ny);  // Distance from the center

	
	float falloff = 1.0f - smoothstep(0.7f, 1.0f, distance);

	float adjustedFalloff = std::min(falloff, (waterHeight - minHeight) / (maxHeight - minHeight));

	return adjustedFalloff;
}



void MidpointDispTerrain::ApplyIslandFalloff(float scale) {
	float waterHeight = GetWaterHeight(); 

	for (int y = 0; y < m_terrainSize; ++y) {
		for (int x = 0; x < m_terrainSize; ++x) {
			float falloff = RadialFalloff(x, y, m_terrainSize, 0.0f, m_maxHeight, waterHeight);
			float currentHeight = m_heightMap.Get(x, y);

			float newHeight = currentHeight * falloff;
			newHeight = std::min(newHeight, waterHeight - 1.0f);  
			m_heightMap.Set(x, y, newHeight);
		}
	}
}

void MidpointDispTerrain::AdjustOutterHeightMap(float waterHeight) {
	int borderSize = m_terrainSize / 16; 

	for (int y = 0; y < m_terrainSize; ++y) {
		for (int x = 0; x < m_terrainSize; ++x) {
			float currentHeight = m_heightMap.Get(x, y);

			float distanceToEdge = std::min({ x, y, m_terrainSize - x - 1, m_terrainSize - y - 1 });
			float borderThreshold = m_terrainSize / 16.0f;

			if (distanceToEdge < borderThreshold) {
				float newHeight = std::min(currentHeight, waterHeight - 1.0f);
				m_heightMap.Set(x, y, newHeight);
			}
		}
	}
}