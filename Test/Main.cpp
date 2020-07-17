#include <Platform/Platform.h>
#include <GFX/RenderCore.h>
#include <Platform/PC/Window.h>
#include <GFX/GFXWrapper.h>
#include <GFX/2D/TilemapAnim.h>
#include <GFX/UI/TextRenderer.h>
using namespace Stardust;

int main() {
	Platform::initPlatform();

	float r = 0.0f;

	unsigned int myTex = GFX::g_TextureManager->loadTex("./terrain_atlas.png", GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, true);
	
	GFX::Render2D::TilemapAnim* tmap = new GFX::Render2D::TilemapAnim(new GFX::TextureAtlas(32), myTex);
	srand(time(0));
	for (int i = 0; i < 1000; i++) {
		GFX::Render2D::TileAnim* tile = new GFX::Render2D::TileAnim();

		tile->offset = { rand() % 480, rand() % 272 };
		tile->extent = { 16, 16 };
		tile->r = rand() % 255;
		tile->g = rand() % 255;
		tile->b = rand() % 255;
		tile->a = rand() % 255;
		tile->layer = 0;
		tile->rotation = rand() % 3;
		tile->texIndex = rand() % 512;
		tile->physics = true;

		tile->isAnim = true;
		tile->animLength = 32;
		tile->indexStart = tile->texIndex;
		tile->tickNumber = 0;

		tmap->addTile(tile);
	}

	tmap->buildMap();

	GFX::UI::TextRenderer* txt = new GFX::UI::TextRenderer();
	txt->init("./assets/font.pgf");
	
	while (true) {
		GFX::g_RenderCore->beginFrame();

		r += 0.005f;
		if (r >= 1.0f) {
			r = 0.0f;
			//tmap->tickPhase();
		}

		GFX::g_RenderCore->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		GFX::g_RenderCore->clear();

		//Main loop
		tmap->drawMap();
		txt->draw("HELLO WORLD", { 240, 136 });

		Platform::platformUpdate();
		GFX::g_RenderCore->endFrame();
	}

	Platform::exitPlatform();
	return 0;
}