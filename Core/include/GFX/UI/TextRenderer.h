#pragma once
#include <string>
#include <GFX/GFXWrapper.h>
#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <intraFont.h>
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
}