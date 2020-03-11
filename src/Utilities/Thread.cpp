#include <Utilities/Thread.h>

namespace Stardust::Utilities {
	Thread::Thread(ThreadHandler th)
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		thr = sceKernelCreateThread("stardust_worker_thread", th, 0x18, 0x10000, 0, NULL);
#endif
	}
	Thread::~Thread()
	{
	}
	void Thread::Start(void* arg)
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceKernelStartThread(thr, 1, arg);
#endif
		detail::core_Logger->log("Starting A Stardust Helper Thread!");
	}
	void Thread::Kill()
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceKernelTerminateDeleteThread(thr);
#endif
		detail::core_Logger->log("Killing A Stardust Helper Thread!");
	}
}