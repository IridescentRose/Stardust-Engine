#pragma once
#include <Platform/Platform.h>
#include <Utilities/Logger.h>

#if CURRENT_PLATFORM != PLATFORM_PSP
	#include <thread>
#endif


namespace Stardust::Utilities {

#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
	typedef int (*ThreadHandler)(SceSize argc, void* argv);
#else
	typedef int (*ThreadHandler)();
#endif

	class Thread {
	public:
		Thread(ThreadHandler th);
		~Thread();

		void Kill();

	private:
#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
		SceUID thr;
#else
		std::thread thr;
#endif

	};
}