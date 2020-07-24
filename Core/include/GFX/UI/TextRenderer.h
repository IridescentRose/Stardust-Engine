#pragma once
#include <string>
#include <GFX/GFXWrapper.h>
#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <intraFont.h>
#else
#define GLT_MANUAL_VIEWPORT
#include <gltext.h>
#endif

#if CURRENT_PLATFORM == PLATFORM_PSP

#define TEXT_RENDERER_TOP	 -1
#define TEXT_RENDERER_BOTTOM -1
#define TEXT_RENDERER_LEFT	 INTRAFONT_ALIGN_LEFT
#define TEXT_RENDERER_CENTER INTRAFONT_ALIGN_CENTER
#define TEXT_RENDERER_RIGHT	 INTRAFONT_ALIGN_RIGHT
#else
#define TEXT_RENDERER_TOP	 GLT_TOP
#define TEXT_RENDERER_BOTTOM GLT_BOTTOM
#define TEXT_RENDERER_LEFT	 GLT_LEFT
#define TEXT_RENDERER_CENTER GLT_CENTER
#define TEXT_RENDERER_RIGHT	 GLT_RIGHT
#endif

namespace Stardust::GFX::UI{

	struct FontStyle{
		uint8_t r, g, b, a;
		float scale;
		int align_v, align_h; //Align V is GLText only
		float angle; //Intrafont only
		bool shadow; //Intrafont only
	};

	class TextRenderer{
	public:
		TextRenderer();
		~TextRenderer();

		void init(std::string font);
		void cleanup();

		void setStyle(FontStyle data);

		void draw(std::string text, glm::ivec2 pos);

	private:
#if CURRENT_PLATFORM == PLATFORM_PSP
		intraFont* font;
#endif
		FontStyle style;
	};

	extern int textVShift;
}