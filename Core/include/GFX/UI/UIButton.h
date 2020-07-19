#pragma once
#include "TextRenderer.h"
#include <GFX/2D/Sprite.h>

namespace Stardust::GFX::UI{
	
	typedef void (*ClickHandler)(void);
	
	class UIButton{
	public:

		UIButton(Render2D::Sprite* sel, Render2D::Sprite* unsel, glm::vec2 size, TextRenderer* text, ClickHandler ch);


		void select();
		void unselect();
		void click();

		void update();
		void draw();

		void setPosition(int x, int y);
		void setSize(glm::vec2 size);
		void setText(std::string text, FontStyle fs);

	private:

		Render2D::Sprite* selected, *unselected;
		TextRenderer* textRender;
		ClickHandler handler;
		std::string text;
		FontStyle style;
		bool isSelected;
		glm::vec2 hitbox;
		glm::vec2 cursorLastPos;
		glm::vec2 position;
	};
}