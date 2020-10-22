#pragma once
#include <Platform/Platform.h>
#include <chrono>
#if CURRENT_PLATFORM == PLATFORM_PSP
#include <ctime>
#include <psptypes.h>
#include <psprtc.h>
#elif CURRENT_PLATFORM == PLATFORM_VITA
#include <ctime>
#include <vitasdk.h>
#endif

namespace Stardust::Utilities {

	/**
	 * Timer class which contains utilities for delta and elapsed time.
	**/
	class Timer {
	public:
		/**
		 * Default constructor obtains current time and sets as base
		**/
		Timer();

		/**
		 * Sets elapsed time to 0, and last time to now.
		**/
		auto reset() -> void;

		/**
		 * Returns the time since last deltaTime call / reset / construction.
		 * This also updates the elapsed(); counter
		**/
		auto deltaTime() -> double;
		
		/**
		 * Total time since starting - requires deltaTime to be called beforehand to be updated
		**/
		auto elapsed() -> double;
		
	private:

		double dt;
		double total;
		std::chrono::time_point<std::chrono::high_resolution_clock> last;

#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
#if CURRENT_PLATFORM == PLATFORM_PSP
		u64 timeCurrent;
		u64 timeLast;
#else
		SceRtcTick timeCurrent;
		SceRtcTick timeLast;
#endif

		u32 tickResolution;
#endif
		
	};

	extern Timer g_AppTimer;
}
