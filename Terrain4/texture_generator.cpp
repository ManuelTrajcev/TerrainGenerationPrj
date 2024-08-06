#include <stdio.h>
#include <stdlib.h>

#include "texture_generator.h"
#include "terrain.h"
#include "ogldev_stb_image.h"

#include "3rdparty/stb_image_write.h"


Texture* TextureGenerator::GenerateTexture(int TextureSize, BaseTerrain* pTerrain, float MinHeight, float MaxHeight) {
	if (m_numTextureTiles == 0) {
		printf("%s:%d: no texture tiles loaded\n", __FILE__, __LINE__);
		exit(0);
	}
	CalculateTextureRegions(MinHeight, MaxHeight);  //

	int BPP = 3;
	int TextureBytes = TextureSize * TextureSize * BPP;
	unsigned char* pTextureData = (unsigned char*)malloc(TextureBytes);
	unsigned char* p = pTextureData;
	float HeightMapToTextureRatio = (float)pTerrain->GetSize() / (float)TextureSize;

    for (int y = 0; y < TextureSize; y++) {
        for (int x = 0; x < TextureSize; x++) {

            float InterpolatedHeight = pTerrain->GetHeightInterpolated((float)x * HeightMapToTextureRatio,      //Interpolacija na visinata od 4 kosinja na kvadrat
                (float)y * HeightMapToTextureRatio);

            float Red = 0.0f;
            float Green = 0.0f;
            float Blue = 0.0f;

            for (int Tile = 0; Tile < m_numTextureTiles; Tile++) {
                Vector3f Color = m_textureTiles[Tile].Image.GetColor(x, y);

                float BlendFactor = RegionPercent(Tile, InterpolatedHeight);        //Vo zavisnost od visinata pravi razlicno mesanje od teksturite

                Red += BlendFactor * Color.r;
                Green += BlendFactor * Color.g;
                Blue += BlendFactor * Color.b;
            }

            if (Red > 255.0f || Green > 255.0f || Blue > 255.0f) {
                printf("%d:%d: %f %f %f\n", y, x, Red, Green, Blue);
                exit(0);
            }

            p[0] = (unsigned char)Red;
            p[1] = (unsigned char)Green;
            p[2] = (unsigned char)Blue;

            p += 3;
        }
    }

    Texture* pTexture = new Texture(GL_TEXTURE_2D);
    stbi_write_png("texture.png", TextureSize, TextureSize, BPP, pTextureData, TextureSize * BPP);
    pTexture->LoadRaw(TextureSize, TextureSize, BPP, pTextureData);
    free(pTextureData);
    return pTexture;
}