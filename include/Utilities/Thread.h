#pragma once
#include <Platform/Platform.h>
#include <Utilities/Logger.h>

#if CURRENT_PLATFORM != PLATFORM_PSP
	#include <thread>
#endif


namespace Stardust::Utilities {

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <pspthreadman.h>
	typedef int (*ThreadHandler)(SceSize argc, void* argv);
#else
	typedef int (*ThreadHandler)(unsigned int argc, void* argv);
#endif

	class Thread {
	public:
		Thread(ThreadHandler th);
		~Thread();

		void Start(void* arg);
		void Kill();

	private:
#if CURRENT_PLATFORM == PLATFORM_PSP
		SceUID thr;
#else
		std::thread thr;
#endif

	};
}