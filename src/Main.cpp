#include <Platform/Platform.h>
#include <Graphics/2D/TilemapAnim.h>
#include <Graphics/2D/CharacterSprite.h>
#include <Utilities/Input.h>

PSP_MODULE_INFO("Stardust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

using namespace Stardust;

int main() {
	Platform::initPlatform();

	Graphics::Texture* texExample = Graphics::TextureUtil::LoadPng("./terrain_atlas.png", true);
	Graphics::TextureAtlas* tileAtlas = new Graphics::TextureAtlas(32);
	Graphics::Render2D::TilemapAnim* tmap = new Graphics::Render2D::TilemapAnim(tileAtlas, texExample);

	Graphics::Texture* charTex = Graphics::TextureUtil::LoadPng("./link.png");
	Graphics::TextureAtlas* charAtlas = new Graphics::TextureAtlas({13.0f, 13.0f});
	Graphics::Render2D::CharacterSprite* charSprite = new Graphics::Render2D::CharacterSprite({ 40, 40 }, charAtlas, charTex);

	Graphics::Render2D::CharacterAnimInfo* info = new Graphics::Render2D::CharacterAnimInfo();
	info->indexStart = 13;
	info->animLength = 3;
	charSprite->addAnimEvent("default", info);

	Graphics::Render2D::CharacterAnimInfo* info2 = new Graphics::Render2D::CharacterAnimInfo();
	info2->indexStart = 13 * 5;
	info2->animLength = 4;

	charSprite->addAnimEvent("walk", info2);

	charSprite->setDefaultAnimEvent("default");
	charSprite->triggerAnimEvent("walk");
	charSprite->position(240, 136);
	charSprite->setLayer(10);

	for (int i = 0; i < 1000; i++) {
		Graphics::Render2D::TileAnim* t = new Graphics::Render2D::TileAnim();
		t->offset = { rand() % 480, rand() % 272 };
		t->extent = { 16, 16 };
		t->rgba = 0xFFFFFFFF;
		t->layer = 0;
		t->texIndex = rand() % 512;
		t->rotation = 0;
		t->isAnim = false;

		tmap->addTile(t);
	}

	tmap->buildMap();
	Graphics::g_RenderCore.Set2DMode();
	int count = 0;

	while (!Utilities::KeyPressed(PSP_CTRL_START)) {
		Graphics::g_RenderCore.BeginCommands();
		Graphics::g_RenderCore.Clear();

		if (count % 15 == 0) {
			charSprite->tickPhase();
		}
		count++;

		charSprite->draw();
		//tmap->drawMap();
		
		Graphics::g_RenderCore.EndCommands();
		Platform::platformUpdate();
	}
	
	delete tmap;
	delete texExample;
	delete tileAtlas;

	Platform::exitPlatform();
	return 0;
}