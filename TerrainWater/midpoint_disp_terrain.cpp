#include "midpoint_disp_terrain.h"
#include <cmath>

void MidpointDispTerrain::CreateMidpointDisplacement(int TerrainSize, int PatchSize, float Roughness, float MinHeight, float MaxHeight)
{
	if (Roughness < 0.0f) {
		printf("%s: roughness must be positive - %f\n", __FUNCTION__, Roughness);
		exit(0);
	}

	m_terrainSize = TerrainSize;
	m_patchSize = PatchSize;
	m_maxHeight = MaxHeight;

	SetMinMaxHeight(MinHeight, MaxHeight);

	m_heightMap.InitArray2D(TerrainSize, TerrainSize, 0.0f);

	CreateMidpointDisplacementF32(Roughness);
	for (int i = 0; i < 10; i++)		//smoothening interations
	{
		SmoothHeightMap(0.005);		//smoothening factor
	}

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

	for (int y = 0; y < m_terrainSize; y += RectSize) {
		for (int x = 0; x < m_terrainSize; x += RectSize) {
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
			value = std::min(value, m_maxHeight); // Clamp the height value to m_maxHeight

			if (value < m_maxHeight * 0.5) {		//Check this!!!!!
				falloff *= 2;
			}
			if (x == 0 || x == m_terrainSize - 1 || y == 0 || y == m_terrainSize - 1) {
				value = 0;
			}

			m_heightMap.Set(mid_x, mid_y, value);
		}
	}
}

void MidpointDispTerrain::SquareStep(int RectSize, float CurHeight)
{
	int HalfRectSize = RectSize / 2;
	float maxDistance = sqrt(2.0f) * m_terrainSize / 2.0f; // Max distance from the center

	for (int y = 0; y < m_terrainSize; y += RectSize) {
		for (int x = 0; x < m_terrainSize; x += RectSize) {
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
			valueTop = std::min(valueTop, m_maxHeight); // Clamp the height value to m_maxHeight

			float valueLeft = CurLeftMid * falloffLeft;
			valueLeft = std::min(valueLeft, m_maxHeight); // Clamp the height value to m_maxHeight

			if (x == 0 || x == m_terrainSize - 1 || y == 0 || y == m_terrainSize - 1) {
				valueTop = 0;
				valueLeft = 0;
			}

			m_heightMap.Set(mid_x, y, valueTop);
			m_heightMap.Set(x, mid_y, valueLeft);
		}
	}
}


void MidpointDispTerrain::SmoothHeightMap(float threshold) {
	Array2D<float> tempMap = m_heightMap;

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

			// maximum difference between the current value and any neighbor
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

				if (currentValue < m_maxHeight*0.5)
				{
					newValue = neighborAvg ;
				}
				else
				{
					newValue = smoothFactor * neighborAvg + (1.0f - smoothFactor) * currentValue;
				}


				//Option 2
				//newValue = neighborAvg;

				tempMap.Set(x, y, newValue);
			}


		}
	}

	m_heightMap = tempMap;
}





