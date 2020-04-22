#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspmath.h>

PSP_MODULE_INFO("Stardust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

#endif


#include <Utilities/Timer.h>
#include <Utilities/Logger.h>
#include <Utilities/Input.h>
#include <Graphics/2D/SpriteAdvanced.h>
#include <perflib.h>

using namespace Stardust;

int main() {

	Platform::initPlatform();

	PFL_Init(false);
	
	Graphics::Texture* texExample = Graphics::TextureUtil::LoadPng("./test.png", true);

#define TESTING_NUMBER 1024

	Graphics::Render2D::Sprite2* spr[TESTING_NUMBER];

	srand(time(0));

	for (int i = 0; i < TESTING_NUMBER; i++) {
		spr[i] = new Graphics::Render2D::Sprite2(texExample, { 16, 16 });
	}


	for (int i = 0; i < TESTING_NUMBER; i++) {
		spr[i]->position(rand() % 480, rand() % 272);
	}

	for (int i = 0; i < TESTING_NUMBER; i++) {
		spr[i]->setColor(rand() % 255, rand() % 255, rand() % 255, 255);
	}

	sceKernelDcacheWritebackInvalidateAll();

	Graphics::g_RenderCore.Set2DMode();

	while (!Utilities::KeyPressed(PSP_CTRL_START)) {

		PFL_BeginCPURecord();
		Graphics::g_RenderCore.BeginCommands();
		Graphics::g_RenderCore.Clear();

		for (int i = 0; i < TESTING_NUMBER; i++) {
			spr[i]->draw();
		}

		
		Graphics::g_RenderCore.EndCommands();
		PFL_EndCPURecord();

		double dt = PFL_GetCPUTime() / 1000.0;
		double fps = 1.0 / dt;

		Utilities::app_Logger->log("FPS: " + std::to_string(fps));
		Utilities::app_Logger->log("POLYS: " + std::to_string(TESTING_NUMBER * 2));
		Platform::platformUpdate();
		sceKernelDcacheWritebackInvalidateAll();
	}
	

	Platform::exitPlatform();

	return 0;
}