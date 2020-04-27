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
#include <Graphics/2D/Tilemap.h>
#include <perflib.h>

using namespace Stardust;

int main() {

	Platform::initPlatform();

	PFL_Init(false);
	PFL_SetTargetFrameRate(60);
	
	Graphics::Texture* texExample = Graphics::TextureUtil::LoadPng("./test.png", true);

#define TESTING_NUMBER 19000

	Graphics::TextureAtlas* tileAtlas = new Graphics::TextureAtlas(1);

	Graphics::Render2D::Tilemap* tmap = new Graphics::Render2D::Tilemap(tileAtlas, texExample);

	for (int i = 0; i < TESTING_NUMBER; i++) {
		Graphics::Render2D::Tile* t = new Graphics::Render2D::Tile();
		t->offset = { rand() % 480, rand() % 272 };
		t->extent = { 16, 16 };
		t->rgba = GU_RGBA(rand() % 255, rand() % 255, rand() % 255, 255);
		t->texIndex = 1;

		tmap->addTile(t);
	}
	
	tmap->buildMap();

	sceKernelDcacheWritebackInvalidateAll();

	Graphics::g_RenderCore.Set2DMode();

	float readingTime = 0.0f;
	int fps = 0;

	while (!Utilities::KeyPressed(PSP_CTRL_START)) {

		PFL_BeginCPURecord();
		Graphics::g_RenderCore.BeginCommands();
		Graphics::g_RenderCore.Clear();

		tmap->drawMap();
		
		Graphics::g_RenderCore.EndCommands();
		PFL_EndCPURecord();

		Platform::platformUpdate();

		double dt = PFL_GetCPUTime() / 1000.0;
		readingTime += dt;
		if (readingTime >= 1.0) {
			readingTime = 0;
			Utilities::app_Logger->log("FPS: " + std::to_string(fps));
			fps = 0;
		}
		else {
			fps++;
		}

	}
	

	Platform::exitPlatform();

	return 0;
}