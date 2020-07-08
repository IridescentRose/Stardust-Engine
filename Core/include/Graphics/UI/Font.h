#pragma once

#include <intraFont.h>

namespace Stardust::Graphics::UI {
	
	struct FontStyle {
		float size;
		unsigned int color;
		unsigned int options;
	};

	extern intraFont* g_DefaultFont;
}