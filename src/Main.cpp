#include <pspkernel.h>
#include <pspdebug.h>
#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP

PSP_MODULE_INFO("Stardust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

#endif

using namespace Stardust;

int main() {

	Platform::initPlatform();

	

	Platform::exitPlatform();

	return 0;
}