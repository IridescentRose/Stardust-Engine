#pragma once
#include "UIText.h"
#include "../2D/SpriteAdvanced.h"

namespace Stardust::Graphics::UI {

	typedef void (*ClickAction)();

	class UIButton {
	public:
		UIButton(glm::vec2 position, glm::vec2 size, Texture* unselected, Texture* selected, UIText* text);

		void setPosition(glm::vec2 position);
		void setText(std::string txt);
		void setText(UIText* text);
		void setSelected();
		void setUnselected();
		void setOffset(int);
		void setTextRelative(glm::vec2);

		void setColor(char r, char g, char b, char a);

		void setClickAction(ClickAction action);

		void click();

		void draw();

	private:
		UIText* txt;
		Render2D::Sprite2* sprt;
		int offset;
		glm::vec2 relative;
		ClickAction action;

		Texture* unsel;
		Texture* sel;

	};
}