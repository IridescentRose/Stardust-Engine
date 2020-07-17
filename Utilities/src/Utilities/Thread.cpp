#include <Utilities/Thread.h>

namespace Stardust::Utilities {
	Thread::Thread(ThreadHandler th)
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		thr = sceKernelCreateThread("stardust_worker_thread", th, 0x18, 0x10000, 0, NULL);
#elif CURRENT_PLATFORM == PLATFORM_VITA
		thr = sceKernelCreateThread("stardust_worker_thread", th, 0x18, 0x10000, 0, 0, NULL);
#else
		thr = std::thread(th);
#endif
		detail::core_Logger->log("Starting A Stardust Helper Thread!");
#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
		sceKernelStartThread(thr, 0, NULL);
#endif
	}
	Thread::~Thread()
	{
	}

	void Thread::Kill()
	{
		detail::core_Logger->log("Killing A Stardust Helper Thread!");
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceKernelTerminateDeleteThread(thr);
#elif CURRENT_PLATFORM == PLATFORM_VITA
		sceKernelDeleteThread(thr);
#else
		thr.join();
#endif
	}
}