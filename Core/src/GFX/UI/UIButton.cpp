#include <GFX/UI/UIButton.h>
#include <Utilities/Input.h>

namespace Stardust::GFX::UI{
	UIButton::UIButton(Render2D::Sprite* sel, Render2D::Sprite* unsel, glm::vec2 size, TextRenderer* fr, ClickHandler ch)
	{
		hitbox = size;
		selected = sel;
		unselected = unsel;
		textRender = fr;
		handler = ch;
		isSelected = false;
		text = "example";
		style = { 255, 255, 255, 255, 1.0f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0.0f, true };
		cursorLastPos = { -1, -1 };
		position = { 0, 0 };
	}
	void UIButton::select()
	{
		isSelected = true;
	}
	void UIButton::unselect()
	{
		isSelected = false;
	}
	void UIButton::click()
	{
		handler();
	}

	void UIButton::update()
	{
#if CURRENT_PLATFORM != PLATFORM_PSP
		glm::vec2 newPos = Utilities::getCursorPos();

		glm::vec2 windowSize = { Platform::PC::g_Window->getWidth(), Platform::PC::g_Window->getHeight() };

		newPos /= windowSize;

		glm::vec2 scale = { 480, 272 };

		newPos *= scale;

		if(newPos != cursorLastPos){
			cursorLastPos = newPos;
			isSelected = false;
		}

		if(newPos.x > position.x - hitbox.x / 2.0f && newPos.x < position.x + hitbox.x / 2.0f){
			if (newPos.y > position.y - hitbox.y / 2.0f && newPos.y < position.y + hitbox.y / 2.0f) {
				isSelected = true;
				if(glfwGetMouseButton(Platform::PC::g_Window->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
					click();
				}
			}
		}
#endif
	}

	void UIButton::draw()
	{
		selected->setPosition(position.x, position.y);
		unselected->setPosition(position.x, position.y);
		if(isSelected){
			selected->draw();
		}else{
			unselected->draw();
		}
		textRender->setStyle(style);
		textRender->draw(text, {position.x, position.y});
	}

	void UIButton::setPosition(int x, int y)
	{
		position = { x, y };
	}
	void UIButton::setSize(glm::vec2 size)
	{
		hitbox = size;
	}
	void UIButton::setText(std::string t, FontStyle fs)
	{
		text = t;
		style = fs;
	}
}