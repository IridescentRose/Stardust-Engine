#pragma once
#include "Font.h"
#include <string>
#include <Math/Vector.h>

namespace Stardust::Graphics::UI {
	class UIText {
	public:

		UIText(glm::vec2 position, std::string content, intraFont* font = g_DefaultFont);

		void setContent(std::string content);
		void setPosition(glm::vec2 pos);
		void setOptions(FontStyle f);

		void draw();

	private:
		glm::vec2 position;
		std::string text;
		intraFont* font;
		FontStyle style;
	};
}