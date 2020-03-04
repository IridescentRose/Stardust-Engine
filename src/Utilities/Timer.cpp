#include <Utilities/Timer.h>

namespace Stardust::Utilities {
	
	Timer::Timer() {
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceRtcGetCurrentTick(&timeLast);
		tickResolution = sceRtcGetTickResolution();
#endif

		total = 0;
		dt = 0;
	}
	
	void Timer::reset() {
		total = 0;
	}

	double Timer::deltaTime() {

#if CURRENT_PLATFORM == PLATFORM_PSP
		sceRtcGetCurrentTick(&timeCurrent);
		dt = (double)(timeCurrent - timeLast) / ((double)tickResolution);
		timeLast = timeCurrent;
#else
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