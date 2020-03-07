#pragma once

#define PLATFORM_PSP 0
#define PLATFORM_PC 1
#define PLATFORM_OTHER 2

//Switch this line of code for other build support
#define CURRENT_PLATFORM PLATFORM_PSP

namespace Stardust::Platform {
#ifndef CURRENT_PLATFORM
#error No Platform was Defined!
#endif

	/*
	Init Platform performs all needed platform initialization code
	*/
	void initPlatform(const char* appName = "APP");

	/*
	Exit Platform performs all needed platform exit code (this is for forced exits)
	*/
	void exitPlatform();
}