#include <pspkernel.h>
#include <pspdebug.h>
#include <Utilities/Common.h>

PSP_MODULE_INFO("Stardust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

int main() {
	pspDebugScreenInit();
	SetupCallbacks();
	pspDebugScreenPrintf("Hello World!");
	sceKernelDelayThread(1000 * 1000 * 3);
	sceKernelExitGame();
	return 0;
}