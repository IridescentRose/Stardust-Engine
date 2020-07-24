#include <Platform/Graphics.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <GFX/GFXWrapper.h>

#if CURRENT_PLATFORM == PLATFORM_PSP

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <psppower.h>
#include <psptypes.h>
#include <pspge.h>
#include <psputils.h>
#include <stdarg.h>
#include <intraFont.h>
#include <Platform/PSP/vram.h>

#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
#include <Platform/PC/Window.h>
namespace Stardust::GFX{
	GLuint program;
	glm::mat4 _gfx_proj, _gfx_view, _gfx_model;
	std::vector<glm::mat4> _matrixStack;
}

#endif

namespace Stardust::GFX{
	TextureManager* g_TextureManager;
}

namespace Stardust::Platform::detail::Graphics {

#if CURRENT_PLATFORM == PLATFORM_PSP
	char list[0x100000] __attribute__((aligned(64)));
	void* _fbp0;
	void* _fbp1;
	void* _zbp;
#endif

#if CURRENT_PLATFORM == PLATFORM_PSP
	void beginFrameDialog() {
		sceGuStart(GU_DIRECT, list);
		sceGuClear(GU_COLOR_BUFFER_BIT);
		sceGuFinish();
		sceGuSync(0, 0);
	}
	void endFrameDialog() {
		sceDisplayWaitVblankStart();
		sceGuSwapBuffers();
	}
#endif

	void initGraphicsContext()
	{
		GFX::g_TextureManager = new GFX::TextureManager();
#if CURRENT_PLATFORM == PLATFORM_PSP
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

		sceGuFrontFace(GU_CCW);
		sceGuEnable(GU_CULL_FACE);
		sceGuEnable(GU_BLEND);
		sceGuEnable(GU_TEXTURE_2D);
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD");
		}
		glViewport(0, 0, PC::g_Window->getWidth(), PC::g_Window->getHeight());
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_MULTISAMPLE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);

		Stardust::GFX::program = Stardust::GFX::loadShaders("./shaders/basic.vert", "./shaders/basic.frag");
		glUseProgram(Stardust::GFX::program);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);


#else
#error No platform graphics context creation exists!
#endif
	}
	void cleanupGraphicsContext()
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceGuTerm();
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
		//Put something here. Platform Exit platform will take care of Platform::PC::g_Window
#else
#error No platform graphics context destruction exists!
#endif
	}
	void beginFrame()
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceGuStart(GU_DIRECT, list);
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
		glViewport(0, 0, PC::g_Window->getWidth(), PC::g_Window->getHeight());
#else
#error No platform graphics begin frame exists!
#endif
	}
	void endFrame()
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceGuFinish();
		sceGuSync(0, 0);
		//VSYNC?
		sceGuSwapBuffers();
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
		Platform::PC::g_Window->draw();
#else
#error No platform graphics begin frame exists!
#endif

	}
}