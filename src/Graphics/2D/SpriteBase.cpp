#include <Graphics/2D/SpriteBase.h>

namespace Stardust::Graphics::Render2D {
	Sprite::Sprite()
	{
		red = green = blue = alpha = 255;
	}

	TextureVertex getVertex(float u, float v, float x, float y, float z)
	{
		TextureVertex vert;
		vert.u = u;
		vert.v = v;
		vert.x = x;
		vert.y = y;
		vert.z = z;
		return vert;
	}

	Sprite::Sprite(Texture* texture)
	{
		tex = texture;

		//generate wertices
		vertices = (TextureVertex*)memalign(16, 4 * sizeof(TextureVertex));

		width = tex->width;
		height = tex->height;

		float hPercent = (float)tex->height / (float)tex->pHeight;
		float wPercent = (float)tex->width / (float)tex->pWidth;

		if (vertices)
		{
			vertices[0] = getVertex(0.0f, 0.0f, -tex->width / 2, -tex->height / 2, 0.0f);
			vertices[1] = getVertex(0.0f, hPercent, -tex->width / 2, tex->height / 2, 0.0f);
			vertices[2] = getVertex(wPercent, 0.0f, tex->width / 2, -tex->height / 2, 0.0f);
			vertices[3] = getVertex(wPercent, hPercent, tex->width / 2, tex->height / 2, 0.0f);
		}

		sceKernelDcacheWritebackInvalidateRange(vertices, 4 * sizeof(TextureVertex));
		red = green = blue = alpha = 255;
	}

	Sprite::Sprite(Texture* texture, int startW, int startH, int endW, int endH)
	{
		tex = texture;

		//generate wertices
		vertices = (TextureVertex*)memalign(16, 4 * sizeof(TextureVertex));

		width = endW;
		height = endH;

		float hstart = (float)startH / (float)tex->pHeight;
		float wstart = (float)startW / (float)tex->pWidth;
		float hPercent = (float)(startH + endH) / (float)tex->pHeight;
		float wPercent = (float)(startW + endW) / (float)tex->pWidth;

		if (vertices)
		{
			vertices[0] = getVertex(wstart, hstart, -width / 2, -height / 2, 0.0f);
			vertices[1] = getVertex(wstart, hPercent, -width / 2, height / 2, 0.0f);
			vertices[2] = getVertex(wPercent, hstart, width / 2, -height / 2, 0.0f);
			vertices[3] = getVertex(wPercent, hPercent, width / 2, height / 2, 0.0f);
		}

		//sceKernelDcacheWritebackInvalidateAll();
		sceKernelDcacheWritebackInvalidateRange(vertices, 4 * sizeof(TextureVertex));
		red = green = blue = alpha = 255;
	}

	Sprite::Sprite(Texture* texture, int startW, int startH, int endW, int endH, bool obr)
	{
		tex = texture;

		//generate wertices
		vertices = (TextureVertex*)memalign(16, 4 * sizeof(TextureVertex));

		width = endW;
		height = endH;

		float hstart = (float)startH / (float)tex->pHeight;
		float wstart = (float)startW / (float)tex->pWidth;
		float hPercent = (float)(startH + endH) / (float)tex->pHeight;
		float wPercent = (float)(startW + endW) / (float)tex->pWidth;

		if (vertices)
		{
			vertices[0] = getVertex(wstart, hstart, 0, 0, 0.0f);
			vertices[1] = getVertex(wstart, hPercent, 0, height, 0.0f);
			vertices[2] = getVertex(wPercent, hstart, width, 0, 0.0f);
			vertices[3] = getVertex(wPercent, hPercent, width, height, 0.0f);
		}

		//sceKernelDcacheWritebackInvalidateAll();
		sceKernelDcacheWritebackInvalidateRange(vertices, 4 * sizeof(TextureVertex));
		red = green = blue = alpha = 255;
	}


	Sprite::~Sprite()
	{
		free(vertices);
	}

	void Sprite::SetPosition(float x, float y)
	{
		posX = x;
		posY = y;
	}

	float Sprite::GetPositionX()
	{
		return posX;
	}

	float Sprite::GetPositionY()
	{
		return posY;
	}

	void Sprite::Scale(float x, float y)
	{
		scaleX = x;
		scaleY = y;

		for (int i = 0; i < 4; i++)
		{
			vertices[i].x = vertices[i].x * scaleX;
			vertices[i].y = vertices[i].y * scaleY;
		}

	}

	void Sprite::Alpha(int _alpha)
	{
		alpha = _alpha;
	}

	void Sprite::RGB(int _red, int _green, int _blue)
	{
		red = _red;
		green = _green;
		blue = _blue;
	}

	void Sprite::Draw()
	{
		sceGumPushMatrix();

		ScePspFVector3 loc = { posX,posY,0.0f };
		sceGumTranslate(&loc);

		sceGuEnable(GU_TEXTURE_2D);

		sceGuColor(GU_RGBA(red, green, blue, alpha));
		tex->bindTexture(GU_NEAREST, GU_NEAREST, true);

		sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 4, 0, vertices);

		sceGuDisable(GU_TEXTURE_2D);
		sceGumPopMatrix();
	}

	void Sprite::RemoveImage()
	{
		delete tex;
	}

}