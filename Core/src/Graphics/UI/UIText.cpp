#include <Graphics/UI/UIText.h>
#include <pspgu.h>
#include <pspgum.h>

namespace Stardust::Graphics::UI {
	UIText::UIText(glm::vec2 p, std::string content, intraFont* f)
	{
		font = f;
		position = p;
		text = content;
		style.color = 0xFFFFFFFF;
		style.size = 1.0f;
		style.options = INTRAFONT_ALIGN_CENTER;
	}
	void UIText::setContent(std::string content)
	{
		text = content;
	}
	void UIText::setPosition(glm::vec2 pos)
	{
		position = pos;
	}
	void UIText::setOptions(FontStyle f)
	{
		style = f;
	}
	void UIText::draw()
	{
		sceGumPushMatrix();
		sceGuEnable(GU_BLEND);
		sceGuEnable(GU_TEXTURE_2D);

		intraFontSetStyle(font, style.size, style.color, 0, 0, style.options);
		intraFontPrint(font, position.x, position.y, text.c_str());

		sceGuDisable(GU_BLEND);
		sceGuDisable(GU_TEXTURE_2D);

		sceGumPopMatrix();
	}
}