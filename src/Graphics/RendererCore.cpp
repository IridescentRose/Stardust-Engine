#include <Graphics/RendererCore.h>
#include "vram.h"


namespace Stardust::Graphics {
	RendererCore::RendererCore()
	{
	}
	RendererCore::~RendererCore()
	{
	}
	void RendererCore::Init()
	{
		_fbp0 = getStaticVramBuffer(BUF_WIDTH, SCR_HEIGHT, GU_PSM_8888);
		_fbp1 = getStaticVramBuffer(BUF_WIDTH, SCR_HEIGHT, GU_PSM_8888);
		_zbp = getStaticVramBuffer(BUF_WIDTH, SCR_HEIGHT, GU_PSM_4444);

		sceGuInit();

		sceGuStart(GU_DIRECT, list);


		sceGuDrawBuffer(GU_PSM_8888, _fbp0, BUF_WIDTH);
		sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, _fbp1, BUF_WIDTH);
		sceGuDepthBuffer(_zbp, BUF_WIDTH);

		sceGuOffset(2048 - (SCR_WIDTH / 2), 2048 - (SCR_HEIGHT / 2));
		sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);

		sceGuDepthRange(50000, 10000);

		sceGuEnable(GU_SCISSOR_TEST);
		sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);
		sceGuEnable(GU_SCISSOR_TEST);
		sceGuDepthFunc(GU_GEQUAL);
		sceGuEnable(GU_DEPTH_TEST);

		sceGuDisable(GU_TEXTURE_2D);
		sceGuEnable(GU_CLIP_PLANES);

		sceGuEnable(GU_CULL_FACE);
		sceGuFrontFace(GU_CCW);

		sceGuEnable(GU_BLEND);
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
		sceGuAlphaFunc(GU_GREATER, 0.0f, 0xff);

		sceGuStencilFunc(GU_ALWAYS, 1, 1); // always set 1 bit in 1 bit mask
		sceGuStencilOp(GU_KEEP, GU_KEEP, GU_REPLACE); // keep value on failed test (fail and zfail) and replace on pass

		sceGuTexFilter(GU_LINEAR, GU_LINEAR);
		sceGuFinish();
		sceGuSync(0, 0);

		sceDisplayWaitVblankStart();
		sceGuDisplay(GU_TRUE);

		sceCtrlSetSamplingCycle(0);
		sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
		sceGuShadeModel(GU_SMOOTH);

		//intraFontInit();
	}
	void RendererCore::Clear()
	{
		sceGuClearStencil(0);
		sceGuClearDepth(0);
		sceGuClear(GU_COLOR_BUFFER_BIT | GU_STENCIL_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
	}
	void RendererCore::BeginCommands(bool dialog)
	{
		sceGuStart(GU_DIRECT, list);
		Clear();
		if (dialog) {
			sceGuFinish();
			sceGuSync(0, 0);
		}
	}
	void RendererCore::EndCommands(bool dialog)
	{
		if (!dialog) {
			sceGuFinish();
			sceGuSync(0, 0);
		}
		sceDisplayWaitVblankStart();
		sceGuSwapBuffers();
	}
	void RendererCore::RenderToTexture(Texture* tex)
	{
		sceGuDrawBufferList(GU_PSM_8888, (void*)tex->data, tex->width);

		sceGuOffset(2048 - (tex->width / 2), 2048 - (tex->height / 2));
		sceGuViewport(2048, 2048, tex->width, tex->height);
	}
	void RendererCore::Blit(int sx, int sy, int sw, int sh, int dx, int dy, int slice)
	{
		int start, end;

		// blit maximizing the use of the texture-cache

		for (start = sx, end = sx + sw; start < end; start += slice, dx += slice)
		{
			TextureVertex* vertices = (TextureVertex*)sceGuGetMemory(2 * sizeof(TextureVertex));
			int width = (start + slice) < end ? slice : end - start;

			vertices[0].u = start;
			vertices[0].v = sy;
			vertices[0].x = dx;
			vertices[0].y = dy;
			vertices[0].z = 0;

			vertices[1].u = start + width;
			vertices[1].v = sy + sh;
			vertices[1].x = dx + width;
			vertices[1].y = dy + sh;
			vertices[1].z = 0;

			sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
		}
	}

#ifdef MC_PSP
	void RendererCore::InitDebugFont()
	{
		fontVerticalShift = 3;
		intraFontInit();
		defFont = intraFontLoad("./assets/font/font.pgf", INTRAFONT_STRING_UTF8 | INTRAFONT_CACHE_LARGE);
		debugFont = intraFontLoad("./assets/font/ltn8.pgf", INTRAFONT_STRING_UTF8 | INTRAFONT_CACHE_LARGE);
		jpn0 = intraFontLoad("./assets/font/jpn0.pgf", INTRAFONT_STRING_UTF8 | INTRAFONT_CACHE_LARGE);
		kr0 = intraFontLoad("./assets/font/kr0.pgf", INTRAFONT_STRING_UTF8 | INTRAFONT_CACHE_LARGE);  //Korean font (not available on all systems) with UTF-8 encoding
		arib = intraFontLoad("./assets/font/arib.pgf", INTRAFONT_STRING_UTF8 | INTRAFONT_CACHE_LARGE);                     //Symbols (not available on all systems)
		chn = intraFontLoad("./assets/font/gb3s1518.bwfon", INTRAFONT_STRING_UTF8 | INTRAFONT_CACHE_LARGE);               //chinese font

		intraFontSetStyle(debugFont, 0.5f, 0xFFFFFFFF, 0, 0.0f, INTRAFONT_ALIGN_CENTER);
		intraFontSetStyle(jpn0, 0.5f, 0xFFFFFFFF, 0, 0.0f, INTRAFONT_ALIGN_CENTER);
		intraFontSetStyle(kr0, 0.5f, 0xFFFFFFFF, 0, 0.0f, INTRAFONT_ALIGN_CENTER);
		intraFontSetStyle(chn, 0.5f, 0xFFFFFFFF, 0, 0.0f, INTRAFONT_ALIGN_CENTER);
		intraFontSetStyle(arib, 0.5f, 0xFFFFFFFF, 0, 0.0f, INTRAFONT_ALIGN_CENTER);
		intraFontSetStyle(defFont, 0.25f, 0xFFFFFFFF, 0, 0.0f, INTRAFONT_ALIGN_CENTER);

		intraFontSetAltFont(defFont, debugFont);
		intraFontSetAltFont(debugFont, jpn0);                     //japanese font is used for chars that don't exist in latin
		intraFontSetAltFont(jpn0, chn);                        //chinese font (bwfon) is used for chars that don't exist in japanese (and latin)
		intraFontSetAltFont(chn, kr0);                         //korean font is used for chars that don't exist in chinese (and jap and ltn)
		intraFontSetAltFont(kr0, arib);                        //symbol font is used for chars that don't exist in korean (and chn,jap & ltn)
	}

	void RendererCore::SetFontStyle(float size, unsigned int color, unsigned int shadowColor, unsigned int options, float angle)
	{
		float size2 = size * 0.5f;
		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);
		intraFontSetStyle(defFont, size2, color, shadowColor, angle, options);

		intraFontSetStyle(debugFont, size2, color, shadowColor, angle, options);
		intraFontSetStyle(jpn0, size2, color, shadowColor, angle, options);
		intraFontSetStyle(kr0, size2, color, shadowColor, angle, options);
		intraFontSetStyle(chn, size2, color, shadowColor, angle, options);
		intraFontSetStyle(arib, size2, color, shadowColor, angle, options);


		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));
	}

	void RendererCore::DebugPrint(int x, int y, const char* message, ...)
	{
		va_list argList;
		char cbuffer[512];
		va_start(argList, message);
		vsnprintf(cbuffer, 512, message, argList);
		va_end(argList);

		sceGuEnable(GU_BLEND);
		sceGuEnable(GU_TEXTURE_2D);

		intraFontPrint(defFont, x, y + fontVerticalShift, cbuffer);
		sceGuDisable(GU_BLEND);
		sceGuDisable(GU_TEXTURE_2D);
	}
#endif

	RendererCore g_RenderCore;
}