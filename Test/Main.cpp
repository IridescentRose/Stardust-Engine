#include <Platform/Platform.h>
#include <Platform/PC/Window.h>
#include <Audio/AudioClip.h>

using namespace Stardust;

int main() {
	Platform::initPC();
	Platform::initPlatform();

	float r = 0;
	while (!Platform::PC::g_Window->shouldClose()) {
		r += 0.0001f;
		if (r > 1.0f) {
			r = 0.0f;
		}
		//Main loop
		Platform::PC::g_Window->update();
		Platform::PC::g_Window->draw();
	}

	Platform::exitPlatform();
	return 0;
}