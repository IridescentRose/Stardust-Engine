#include <Utilities/Timer.h>

namespace Stardust::Utilities {
	
	Timer::Timer() {
		last = std::chrono::high_resolution_clock::now();
		total = 0;
		dt = 0;
	}
	
	void Timer::reset() {
		total = 0;
	}

	double Timer::deltaTime() {
		std::chrono::time_point current = std::chrono::steady_clock::now();
		dt = std::chrono::duration_cast<std::chrono::duration<double, std::deci>>(current - last).count();
		last = current;

		total += dt;
		return dt;
	}
	
	double Timer::elapsed() {
		return total;
	}

	Timer g_AppTimer;

}