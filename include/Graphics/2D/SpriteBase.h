#pragma once
#include <Graphics/TextureUtil.h>
#include <Graphics/RenderTypes.h>

namespace Stardust::Graphics::Render2D {
	class Sprite
	{
	public:
		Sprite();
		Sprite(Texture* texture);
		Sprite(Texture* texture, int startW, int startH, int endW, int endH);
		Sprite(Texture* texture, int startW, int startH, int endW, int endH, bool obr);
		~Sprite();


		void SetPosition(float x, float y);
		void Scale(float x, float y);

		void RemoveImage();
		void Draw();

		void Alpha(int _alpha);
		void RGB(int _red, int _green, int _blue);

		float GetPositionX();
		float GetPositionY();


		Texture* tex;
		TextureVertex* vertices;
		bool noTex;
		float scaleX, scaleY;
		float posX, posY;
		int width, height;
		int red, green, blue, alpha;
	};
}