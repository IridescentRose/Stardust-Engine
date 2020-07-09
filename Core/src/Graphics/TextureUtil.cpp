#include <Graphics/TextureUtil.h>
#include "vram.h"
#include <Utilities/Logger.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Stardust::Graphics {
	bool FileExist(std::string fileName)
	{
		FILE* fp;

		if ((fp = fopen(fileName.c_str(), "r")) == NULL)
		{
			return false;
		}

		fclose(fp);

		return true;
	}

	
	Texture* TextureUtil::LoadTex(std::string fileName, bool vram)
	{
		return LoadTex(fileName.c_str(), GU_PSM_8888, 1, vram);
	}
	Texture* TextureUtil::LoadTex(const char* filename, int ColorMode, int Swizzle, bool Vram)
	{

		int OutBytesPerPixel = 4;
		int Power2Width = 0;
		int Power2Height = 0;

		//GET WIDTH / HEIGHT
		int width, height, channels;
		unsigned short* data = (unsigned short*)stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

		Power2Width = powerOfTwo(width);
		Power2Height = powerOfTwo(height);

		Texture* Image1 = new Texture();

		Image1->width = width;
		Image1->height = height;
		Image1->pWidth = Power2Width;
		Image1->pHeight = Power2Height;
		Image1->ramSpace = Vram;
		Image1->colorMode = ColorMode;
		Image1->swizzled = Swizzle;
		Image1->fileName = filename;
		Image1->data = data;

		unsigned short* swizzled_pixels = NULL;
		if (Vram)
		{
			swizzled_pixels = (unsigned short*)getStaticVramTexture(Power2Width, Power2Height, ColorMode);
		}
		else
		{
			swizzled_pixels = (unsigned short*)memalign(16, Image1->pHeight * Image1->pWidth * OutBytesPerPixel);
		}

		swizzle_fast((u8*)swizzled_pixels, (const u8*)data, Image1->pWidth * OutBytesPerPixel, Image1->pHeight);

		Image1->data = swizzled_pixels;

		stbi_image_free(data);

		//clear the cache or there will be some errors
		sceKernelDcacheWritebackInvalidateAll();

		return Image1;
	}
	TextureAtlas::TextureAtlas(short tileSide)
	{
		sideCount = { tileSide, tileSide };
	}
	TextureAtlas::TextureAtlas(glm::vec2 sideXY)
	{
		sideCount = sideXY;
	}
	std::array<float, 8> TextureAtlas::getTexture(int index)
	{
		int row = index / (int)sideCount.x;
		int column = index % (int)sideCount.y;

		float sizeX = 1.f / ((float)sideCount.x);
		float sizeY = 1.f / ((float)sideCount.y);
		float y = (float)row * sizeY;
		float x = (float)column * sizeX;
		float h = y + sizeY;
		float w = x + sizeX;

		return {
			x, y,
			w, y,
			w, h,
			x, h,
		};
	}
}