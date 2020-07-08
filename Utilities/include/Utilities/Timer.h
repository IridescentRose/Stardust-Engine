#pragma once
#include <Platform/Platform.h>
#include <chrono>

namespace Stardust::Utilities {

	class Timer {
	public:
		Timer();

		void reset();

		double deltaTime();
		double elapsed();
		
	private:

		double dt;
		double total;
		std::chrono::time_point<std::chrono::high_resolution_clock> last;
		
	};

	extern Timer g_AppTimer;
}