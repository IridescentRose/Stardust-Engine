#include <Graphics/TextureUtil.h>
#include "vram.h"
#include <Utilities/Logger.h>

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

	Texture* TextureUtil::LoadPng(std::string fileName, bool vram)
	{
		return LoadPng(fileName.c_str(), GU_PSM_8888, 1, vram);
	}

	Texture* TextureUtil::LoadPng(const char* filename, int ColorMode, int Swizzle, bool Vram)
	{
		unsigned short* Buffer;
		unsigned short* swizzled_pixels = NULL;

		int OutBytesPerPixel;
		int Power2Width = 0;
		int Power2Height = 0;

		png_structp png_ptr;
		png_infop info_ptr;
		unsigned int sig_read = 0;
		png_uint_32 width, height, x, y;
		int bit_depth, color_type, interlace_type;
		unsigned int* line;
		FILE* fp;

		if (ColorMode == GU_PSM_4444 || ColorMode == GU_PSM_5650 || ColorMode == GU_PSM_5551)
			OutBytesPerPixel = 2;
		else
			OutBytesPerPixel = 4;

		if ((fp = fopen(filename, "rb")) == NULL)
		{
			printf("Can't open file %s\n", filename);
			return NULL;
		}

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL)
		{
			fclose(fp);
			return NULL;
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			fclose(fp);
			png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
			return NULL;
		}

		png_init_io(png_ptr, fp);
		png_set_sig_bytes(png_ptr, sig_read);
		png_read_info(png_ptr, info_ptr);
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);
		png_set_strip_16(png_ptr);
		png_set_packing(png_ptr);

		if (color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png_ptr);
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png_ptr);

		png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

		line = (unsigned int*)malloc(width * 4);
		if (!line)
		{
			fclose(fp);
			png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
			return NULL;
		}

		Power2Width = powerOfTwo(width);
		Power2Height = powerOfTwo(height);
		Buffer = (unsigned short*)memalign(16, Power2Width * Power2Height * OutBytesPerPixel);
		//Buffer = (unsigned short*)malloc(sizeof(unsigned short) * (Power2Width*Power2Height*OutBytesPerPixel));

		for (y = 0; y < height; y++)
		{
			png_read_row(png_ptr, (unsigned char*)line, NULL);

			for (x = 0; x < width; x++)
			{
				unsigned int* Buffer32 = (unsigned int*)Buffer;
				unsigned int color32 = line[x];
				unsigned short color16;

				if (ColorMode == GU_PSM_5551)
				{
					color16 = Color8888To5551(color32);
					Buffer[y * Power2Width + x] = color16;
				}
				else if (ColorMode == GU_PSM_4444)
				{
					color16 = Color8888To4444(color32);
					Buffer[y * Power2Width + x] = color16;
				}
				else if (ColorMode == GU_PSM_5650)
				{
					color16 = Color8888To5650(color32);
					Buffer[y * Power2Width + x] = color16;
				}
				else
				{
					Buffer32[y * Power2Width + x] = color32;
				}
			}
		}

		free(line);
		png_read_end(png_ptr, info_ptr);
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
		fclose(fp);

		Texture* Image1 = new Texture();

		Image1->width = width;
		Image1->height = height;
		Image1->pWidth = Power2Width;
		Image1->pHeight = Power2Height;
		Image1->ramSpace = Vram;
		Image1->colorMode = ColorMode;
		Image1->swizzled = Swizzle;
		Image1->fileName = filename;

		if (Vram)
		{
			swizzled_pixels = (unsigned short*)getStaticVramTexture(Power2Width, Power2Height, ColorMode);//valloc(Image1->power2Height*Image1->power2Width*OutBytesPerPixel);

		}
		else
		{
			swizzled_pixels = (unsigned short*)memalign(16, Image1->pHeight * Image1->pWidth * OutBytesPerPixel);
			//swizzled_pixels = (unsigned short*)malloc(sizeof(unsigned short) * (Power2Width*Power2Height*OutBytesPerPixel));
		}

		swizzle_fast((u8*)swizzled_pixels, (const u8*)Buffer, Image1->pWidth * OutBytesPerPixel, Image1->pHeight);

		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
		//sceKernelDcacheWritebackAll();

		Image1->data = swizzled_pixels;
		free(Buffer);

		//clear the cache or there will be some errors
		sceKernelDcacheWritebackInvalidateAll();

		return Image1;
	}
	TextureAtlas::TextureAtlas(short tileSide)
	{
		sideCount = tileSide;
	}
	std::array<float, 8> TextureAtlas::getTexture(int index)
	{
		int row = index / 32;
		int column = index % 32;

		float size = 1.f / ((float)sideCount);
		float y = (float)row * size;
		float x = (float)column * size;
		float h = y + size;
		float w = x + size;

		return {
			x, y,
			w, y,
			w, h,
			x, h,
		};
	}
}