#define GLT_IMPLEMENTATION
#include <GFX/UI/TextRenderer.h>

namespace Stardust::GFX::UI{


	int textVShift = 4;
	TextRenderer::TextRenderer()
	{

#if CURRENT_PLATFORM == PLATFORM_PSP
		style = { 255, 255, 255, 255, 1.0f, 0, INTRAFONT_ALIGN_CENTER, 0.0f, true };
#else
		style = {255, 255, 255, 255, 1.0f, GLT_CENTER, GLT_CENTER, 0.0f, false};
#endif
	}
	TextRenderer::~TextRenderer()
	{
	}

	void TextRenderer::init(std::string f)
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		font = intraFontLoad(f.c_str(), INTRAFONT_CACHE_ASCII);
		intraFontSetStyle(font, 1.0f, 0xFFFFFFFF, 0xFF333333, 0.0f, INTRAFONT_ALIGN_CENTER);
#else
		gltInit();
		gltViewport(480, 272);
#endif
	}
	void TextRenderer::cleanup()
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		delete font;
#else
		gltTerminate();
#endif
	}
	void TextRenderer::setStyle(FontStyle data)
	{
		style = data;
#if CURRENT_PLATFORM == PLATFORM_PSP
		intraFontSetStyle(font, data.scale, GU_RGBA(data.r, data.g, data.b, data.a), data.shadow ? 0xFF333333 : 0, data.angle, data.align_h);
#endif
	}
	void TextRenderer::draw(std::string text, glm::ivec2 pos)
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		intraFontPrint(font, pos.x, pos.y + textVShift, text.c_str());
#else
		GLTtext* txt = gltCreateText();
		gltSetText(txt, text.c_str());

		glDisable(GL_DEPTH_TEST);
		gltBeginDraw();

		gltColor( (float)style.r / 255.0f, (float)style.g / 255.0f, (float)style.b / 255.0f, (float)style.a / 255.0f);
		gltDrawText2DAligned(txt, pos.x, pos.y, style.scale, style.align_h, style.align_v);

		gltEndDraw();
		glEnable(GL_DEPTH_TEST);

		gltDeleteText(txt);
#endif
	}
}