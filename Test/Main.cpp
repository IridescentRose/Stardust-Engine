#include <Platform/Platform.h>
#include <Platform/PC/Window.h>
#include <Audio/AudioClip.h>

using namespace Stardust;

int main() {
	Platform::initPC();
	Platform::initPlatform();

	Audio::AudioClip* audio = new Audio::AudioClip("runereality.ogg");
	audio->Play();

	while (!Platform::PC::g_Window->shouldClose()) {
		//Main loop
		Platform::PC::g_Window->update();
		Platform::PC::g_Window->draw();
	}

	Platform::exitPlatform();
	return 0;
}