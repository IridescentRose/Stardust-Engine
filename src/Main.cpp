#include <Platform/Platform.h>
#include <Graphics/2D/TilemapAnim.h>
#include <Graphics/2D/CharacterSprite.h>
#include <Utilities/Input.h>
#include <Graphics/2D/TopDownController.h>
#include <Graphics/2D/SideScrollerController.h>
#include <Utilities/Logger.h>
#include <Graphics/UI/UIText.h>
#include <Network/NetworkDriver.h>
PSP_MODULE_INFO("Stardust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

using namespace Stardust;

int main() {
	Platform::initPlatform();

	Network::g_NetworkDriver.Init();
	Network::g_NetworkDriver.GetFileHTTP("http://psphbshop.s3.eu-central-1.amazonaws.com/mcpsp.png", "a.png");

	Platform::exitPlatform();
	return 0;
}