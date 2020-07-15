#include <Platform/Platform.h>
#include <GFX/RenderCore.h>
#include <Platform/PC/Window.h>
#include <GFX/GFXWrapper.h>

using namespace Stardust;

int main() {
	Platform::initPlatform();

	GFX::g_RenderCore->set2DMode(-16.0f / 9.0f, 16.0f / 9.0f, -1.0f, 1.0f, -10.0f, 10.0f);

	float r = 0.0f;

	GFX::Mesh mesh;

	mesh.color = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	mesh.uv = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	mesh.position = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	mesh.indices = {
		0, 1, 2, 2, 3, 0
	};

	GFX::Model model(&mesh);

	unsigned int myTex = GFX::g_TextureManager->loadTex("test.png", GFX_FILTER_LINEAR, GFX_FILTER_LINEAR, true);

	while (true) {
		GFX::g_RenderCore->beginFrame();

		//r += 0.0001f;
		if (r >= 1.0f) {
			r = 0.0f;
		}

		GFX::g_RenderCore->setClearColor(r, 0.f, 1.0f, 1.0f);
		GFX::g_RenderCore->clear();

		//Main loop
		GFX::g_TextureManager->bindTex(myTex);
		model.draw();

		Platform::platformUpdate();
		GFX::g_RenderCore->endFrame();
	}

	Platform::exitPlatform();
	return 0;
}