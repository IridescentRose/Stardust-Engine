#pragma once
#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <ctime>
#include <psptypes.h>
#include <psprtc.h>
#endif

namespace Stardust::Utilities {

	double getTime();

	class Timer {
	public:
		Timer();

		void reset();

		double deltaTime();
		double elapsed();
		
	private:

		double dt;
		double total;
#if CURRENT_PLATFORM == PLATFORM_PSP
		u64 timeCurrent;
		u64 timeLast;

		u32 tickResolution;
#endif
		
	};

	extern Timer g_AppTimer;
}