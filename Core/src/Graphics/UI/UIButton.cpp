#include <Graphics/UI/UIButton.h>

namespace Stardust::Graphics::UI {
	UIButton::UIButton(glm::vec2 position, glm::vec2 size, Texture* unselected, Texture* selected, UIText* text)
	{
		unsel = unselected;
		sel = selected;
		txt = text;

		sprt = new Graphics::Render2D::Sprite2(unselected, position, size);
		action = 0;
		offset = 6;
		relative = { 0, 0 };
	}
	void UIButton::setPosition(glm::vec2 position)
	{
		sprt->position(position.x, position.y);
	}
	void UIButton::setText(std::string text)
	{
		txt->setContent(text);
	}
	void UIButton::setText(UIText* text)
	{
		txt = text;
	}
	void UIButton::setSelected()
	{
		sprt->setTexture(sel);
	}
	void UIButton::setUnselected()
	{
		sprt->setTexture(unsel);
	}
	void UIButton::setOffset(int i)
	{
		offset = i;
	}
	void UIButton::setTextRelative(glm::vec2 v)
	{
		relative = v;
	}
	void UIButton::setColor(char r, char g, char b, char a)
	{
		sprt->setColor(r, g, b, a);
	}
	void UIButton::setClickAction(ClickAction a)
	{
		action = a;
	}
	void UIButton::click()
	{
		if (action != 0) {
			action();
		}
	}
	void UIButton::draw()
	{
		sprt->draw();
		txt->setPosition({ (sprt->getPosition().x + relative.x) * 2.0f, (sprt->getPosition().y + relative.y) * 2.0f + offset });
		txt->draw();
	}
}