#include <Utilities/Timer.h>

namespace Stardust::Utilities {
	
	Timer::Timer() {
#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
		sceRtcGetCurrentTick(&timeLast);
		tickResolution = sceRtcGetTickResolution();
#endif

		last = std::chrono::high_resolution_clock::now();
		total = 0;
		dt = 0;
	}
	
	void Timer::reset() {
		total = 0;
	}

	double Timer::deltaTime() {
#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
		sceRtcGetCurrentTick(&timeCurrent);

#if CURRENT_PLATFORM == PLATFORM_VITA
		dt = (double)(timeCurrent.tick - timeLast.tick) / ((double)tickResolution);
#else
		dt = (double)(timeCurrent - timeLast) / ((double)tickResolution);
#endif
		timeLast = timeCurrent;
#else
		std::chrono::time_point<std::chrono::high_resolution_clock> current = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration_cast<std::chrono::duration<double, std::deci>>(current - last).count();
		last = current;
		//Figure out the DT
#endif

		total += dt;
		return dt;
	}
	
	double Timer::elapsed() {
		return total;
	}

	Timer g_AppTimer;

}