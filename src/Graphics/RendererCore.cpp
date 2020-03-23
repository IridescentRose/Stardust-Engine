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

	RendererCore g_RenderCore;
}