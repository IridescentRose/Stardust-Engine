#include <Platform/Platform.h>
#include <GFX/RenderCore.h>
#include <Platform/PC/Window.h>
#include <GFX/GFXWrapper.h>

#define STBI_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace Stardust;

int powerOfTwo(int value) {
	int poweroftwo = 1;
	while (poweroftwo < value) {
		poweroftwo <<= 1;
	}
	return poweroftwo;
}

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
		1.0f, 0.0f,
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
	
	int width, height, bpp;
	unsigned char* rgb = stbi_load("test.png", &width, &height, &bpp, 4);

	int pWidth = powerOfTwo(width), pHeight = powerOfTwo(height);

	

	while (true) {
		GFX::g_RenderCore->beginFrame();

		r += 0.0001f;
		if (r >= 1.0f) {
			r = 0.0f;
		}

		GFX::g_RenderCore->setClearColor(r, 0.f, 1.0f, 1.0f);
		GFX::g_RenderCore->clear();

		sceGuTexMode(GU_PSM_8888, 0, 0, 0);
		sceGuTexImage(0, pWidth, pHeight, pWidth, rgb);
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);
		sceGuTexScale(1, 1);
		sceGuTexOffset(0, 0);

		//Main loop
		model.draw();

		Platform::platformUpdate();
		GFX::g_RenderCore->endFrame();
	}

	Platform::exitPlatform();
	return 0;
}