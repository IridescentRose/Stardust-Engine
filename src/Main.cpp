#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspmath.h>

PSP_MODULE_INFO("Stardust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

#endif


using namespace Stardust;

int main() {

	Platform::initPlatform();

	pspDebugScreenInit();
	pspDebugScreenPrintf("VFPU RAND: %f", vfpu_randf(0.0f, 100.0f));
	sceKernelDelayThread(1000000);

	Platform::exitPlatform();

	return 0;
}