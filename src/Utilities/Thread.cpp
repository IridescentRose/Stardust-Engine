#include <Utilities/Thread.h>

namespace Stardust::Utilities {
	Thread::Thread(ThreadHandler th)
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		thr = sceKernelCreateThread("stardust_worker_thread", th, 0x18, 0x10000, 0, NULL);
#else
		thr = std::thread(th);
#endif
	}
	Thread::~Thread()
	{
	}
	void Thread::Start(void* arg)
	{
		detail::core_Logger->log("Starting A Stardust Helper Thread!");
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceKernelStartThread(thr, 1, arg);
#endif
	}
	void Thread::Kill()
	{
		detail::core_Logger->log("Killing A Stardust Helper Thread!");
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceKernelTerminateDeleteThread(thr);
#else
		thr.join();
#endif
	}
}