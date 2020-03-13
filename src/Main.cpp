#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspmath.h>

PSP_MODULE_INFO("Stardust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

#endif


#include <Utilities/Timer.h>
#include <Utilities/Logger.h>
#include <Utilities/Input.h>
#include <Profiler/Profiler.h>

using namespace Stardust;

int main() {

	Platform::initPlatform();
	Profiling::Profiler pf("Test");


	pf.beginProfileMethod();
	for(int i = 0; i < 30; i++) {
		pf.beginProfileSubMethod();
		Utilities::app_Logger->log("Hello World!");
		sceKernelDelayThread(rand() % 50 * 1000);
		pf.endProfileSubMethod("TESTING!", "Log");
	}
	pf.endProfileMethod("TESTING!");

	pf.outputStats();

	Platform::exitPlatform();

	return 0;
}