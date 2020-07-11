#include <Platform/Platform.h>
#include <GFX/RenderCore.h>
#include <Platform/PC/Window.h>

using namespace Stardust;

int main() {
	Platform::initPlatform();

	float r = 0.0f;

	while (true) {
		GFX::g_RenderCore->beginFrame();

		r += 0.0001f;
		if (r >= 1.0f) {
			r = 0.0f;
		}

		GFX::g_RenderCore->setClearColor(r, 0.f, 1.0f, 1.0f);
		GFX::g_RenderCore->clear();

		//Main loop


		Platform::platformUpdate();
		GFX::g_RenderCore->endFrame();
	}

	Platform::exitPlatform();
	return 0;
}