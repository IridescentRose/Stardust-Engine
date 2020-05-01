#include <Platform/Platform.h>
#include <Graphics/2D/TilemapAnim.h>
#include <Graphics/2D/CharacterSprite.h>
#include <Utilities/Input.h>
#include <Graphics/2D/Controller2D.h>
#include <Utilities/Logger.h>
PSP_MODULE_INFO("Stardust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

using namespace Stardust;

int main() {
	Platform::initPlatform();

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
	charSprite->setDefaultAnimEvent("walk");
	charSprite->triggerAnimEvent("walk");
	charSprite->position(240, 136);
	charSprite->setLayer(10);

	Graphics::Render2D::Controller2D controller = Graphics::Render2D::Controller2D(charSprite);
	controller.setPosition({ 240, 136 });

	Graphics::g_RenderCore.Set2DMode();

	while (!Utilities::KeyPressed(PSP_CTRL_START)) {
		Graphics::g_RenderCore.BeginCommands();
		Graphics::g_RenderCore.Clear();
		
		controller.update(0.016f);
		controller.draw();
		
		Graphics::g_RenderCore.EndCommands();
		Platform::platformUpdate();
	}

	Platform::exitPlatform();
	return 0;
}