#pragma once
#include <Graphics/RendererCore.h>
#include <string>

namespace Stardust::Graphics {
	union Color {
		unsigned int hex;
		struct components {
			unsigned char a, r, g, b;
		} components;
	};

	typedef struct
	{
		float u, v;
		unsigned int color;
		float x, y, z;
	}__attribute__((packed)) Vertex;
	typedef struct
	{
		float u, v;
		float x, y, z;
	}__attribute__((packed)) TextureVertex;
	typedef struct
	{
		unsigned int color;
		float x, y, z;
	}__attribute__((packed)) ColorVertex;

	class Texture {
	public:
		int width, height, pWidth, pHeight, ramSpace, colorMode, swizzled;

		unsigned int texID;

		std::string fileName;

		unsigned short* data; //32 bit pixel data in (likely) 8888 RGBA

		inline void bindTexture(int minFilter = GU_NEAREST, int maxFilter = GU_NEAREST, bool modulate = false, bool repeat = true) {
			if (modulate) {
				sceGuTexMode(colorMode, 0, 0, swizzled);
				sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
				sceGuTexFilter(minFilter, maxFilter);
				sceGuTexOffset(0.0f, 0.0f);
				sceGuTexWrap(repeat ? GU_REPEAT : GU_CLAMP, repeat ? GU_REPEAT : GU_CLAMP);
				sceGuTexImage(0, pWidth, pHeight, pWidth, data);
			}
			else {
				sceGuTexMode(colorMode, 0, 0, 1);
				sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
				sceGuTexFilter(minFilter, maxFilter);
				sceGuTexOffset(0.0f, 0.0f);
				sceGuTexWrap(repeat ? GU_REPEAT : GU_CLAMP, repeat ? GU_REPEAT : GU_CLAMP);
				sceGuTexImage(0, pWidth, pHeight, pWidth, data);
			}
		}

		inline int getAlpha(int x, int y) {
			float box_width = width / (width * 4 / 16);
			float box_height = height / (height / 8);
			float box_num = width / box_width;

			/// box width = 4
			/// box height = 8
			/// strip = 64
			/// strip number = 4096
			/// box number  = 64

			float box_x = x / box_width;
			box_x = (int)box_x;
			float box_y = y / box_height;
			box_y = (int)box_y;

			int box_number = box_x + (box_y * box_num);
			int box_first_index = box_number * box_width * box_height * 2;

			int x_in_box = x - (box_x * box_width);
			int y_in_box = y - (box_y * box_height);

			int end_index = box_first_index + ((x_in_box * 2) + (y_in_box * box_width * 2));

			return data[end_index + 1] >> 8;
		}
		inline int getGreen(int x, int y) {
			float box_width = width / (width * 4 / 16);
			float box_height = height / (height / 8);
			float box_num = width / box_width;

			/// box width = 4
			/// box height = 8
			/// strip = 64
			/// strip number = 4096
			/// box number  = 64

			float box_x = x / box_width;
			box_x = (int)box_x;
			float box_y = y / box_height;
			box_y = (int)box_y;

			int box_number = box_x + (box_y * box_num);
			int box_first_index = box_number * box_width * box_height * 2;

			int x_in_box = x - (box_x * box_width);
			int y_in_box = y - (box_y * 8);

			int end_index = box_first_index + ((x_in_box * 2) + (y_in_box * box_width * 2));

			return data[end_index] >> 8;
		}
		inline int getRed(int x, int y) {
			float box_width = width / (width * 4 / 16);
			float box_height = height / (height / 8);
			float box_num = width / box_width;


			float box_x = x / box_width;
			box_x = (int)box_x;
			float box_y = y / box_height;
			box_y = (int)box_y;

			int box_number = box_x + (box_y * box_num);
			int box_first_index = box_number * box_width * box_height * 2;

			int x_in_box = x - (box_x * box_width);
			int y_in_box = y - (box_y * box_height);

			int end_index = box_first_index + ((x_in_box * 2) + (y_in_box * box_width * 2));

			return data[end_index];
		}
		inline int getBlue(int x, int y) {
			float box_width = width / (width * 4 / 16);
			float box_height = height / (height / 8);
			float box_num = width / box_width;

			/// box width = 4
			/// box height = 8
			/// strip = 64
			/// strip number = 4096
			/// box number  = 64

			float box_x = x / box_width;
			box_x = (int)box_x;
			float box_y = y / box_height;
			box_y = (int)box_y;

			int box_number = box_x + (box_y * box_num);
			int box_first_index = box_number * box_width * box_height * 2;

			int x_in_box = x - (box_x * box_width);
			int y_in_box = y - (box_y * box_height);

			int end_index = box_first_index + ((x_in_box * 2) + (y_in_box * box_width * 2));

			return data[end_index + 1];
		}
		inline void setColour(int x, int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
			float box_width = width / (width * 4 / 16);
			float box_height = height / (height / 8);
			float box_num = width / box_width;

			/// box width = 4
			/// box height = 8
			/// strip = 64
			/// strip number = 4096
			/// box number  = 64

			float box_x = x / box_width;
			box_x = (int)box_x;
			float box_y = y / box_height;
			box_y = (int)box_y;

			int box_number = box_x + (box_y * box_num);
			int box_first_index = box_number * box_width * box_height * 2;

			int x_in_box = x - (box_x * box_width);
			int y_in_box = y - (box_y * box_height);

			int end_index = box_first_index + ((x_in_box * 2) + (y_in_box * box_width * 2));

			data[end_index] = (red) | (green << 8);
			data[end_index + 1] = (blue) | (alpha << 8);
		}

		~Texture()
		{
			if (data != NULL)
				free(data);
		}
	};
}
