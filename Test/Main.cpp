#include <Platform/Platform.h>
#include <Platform/PC/Window.h>

using namespace Stardust;

int main() {
	Platform::initPlatform();
	Platform::PC::Window* w = new Platform::PC::Window(800, 640, "test", false, false);

	while (!w->shouldClose()) {
		//Main loop
		w->update();
		w->draw();
	}

	Platform::exitPlatform();
	return 0;
}