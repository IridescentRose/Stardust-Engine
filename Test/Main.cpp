#include <Platform/Platform.h>
#include <GFX/RenderCore.h>
#include <Platform/PC/Window.h>
#include <GFX/GFXWrapper.h>

using namespace Stardust;

int main() {
	Platform::initPlatform();

	GFX::g_RenderCore->set2DMode(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	float r = 0.0f;

	GFX::Mesh mesh;

	mesh.color = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
	};

	mesh.uv = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};

	mesh.position = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	mesh.indices = {
		0, 1, 2
	};

	GFX::Model model(&mesh);

	while (true) {
		GFX::g_RenderCore->beginFrame();

		r += 0.0001f;
		if (r >= 1.0f) {
			r = 0.0f;
		}

		GFX::g_RenderCore->setClearColor(r, 0.f, 1.0f, 1.0f);
		GFX::g_RenderCore->clear();

		//Main loop
		model.draw();

		Platform::platformUpdate();
		GFX::g_RenderCore->endFrame();
	}

	Platform::exitPlatform();
	return 0;
}