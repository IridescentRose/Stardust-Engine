#include <Platform/Platform.h>
#include <GFX/RenderCore.h>
#include <Platform/PC/Window.h>
#include <GFX/GFXWrapper.h>
#include <GFX/2D/Sprite.h>
using namespace Stardust;

int main() {
	Platform::initPlatform();

	GFX::g_RenderCore->set2DMode(-16.0f / 9.0f, 16.0f / 9.0f, -1.0f, 1.0f, -10.0f, 10.0f);

	float r = 0.0f;

	unsigned int myTex = GFX::g_TextureManager->loadTex("test.png", GFX_FILTER_LINEAR, GFX_FILTER_LINEAR, true);
	GFX::Render2D::Sprite* sprite = new GFX::Render2D::Sprite(myTex, { 0.5f, 0.5f });

	while (true) {
		GFX::g_RenderCore->beginFrame();

		r += 0.01f;
		if (r >= 1.0f) {
			r = 0.0f;
		}

		GFX::g_RenderCore->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		GFX::g_RenderCore->clear();

		//Main loop
		sprite->setColor(1.0f * r, 0.0f, (1.0f-r) * 1.0f, 1.0f);
		sprite->draw();

		Platform::platformUpdate();
		GFX::g_RenderCore->endFrame();
	}

	Platform::exitPlatform();
	return 0;
}