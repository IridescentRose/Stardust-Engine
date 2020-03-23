#include <Platform/Platform.h>
#include <Utilities/Timer.h>
#include <Utilities/Logger.h>
#include <Utilities/JSON.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <psppower.h>
#include <stdlib.h>
#include <pspkernel.h>
#include <time.h>
#include <pspmath.h>
#include <Audio/sound_utils/oslib.h>
#include <Audio/sound_utils/audio.h>
#endif

namespace Stardust::Platform {
#if CURRENT_PLATFORM == PLATFORM_PSP
	//PSP
	// Exit callback
	int exit_callback(int arg1, int arg2, void* common)
	{
		sceKernelExitGame();
		return 0;
	}

	// Callback thread 
	int CallbackThread(SceSize args, void* argp)
	{
		int cbid;

		cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
		sceKernelRegisterExitCallback(cbid);

		sceKernelSleepThreadCB();

		return 0;
	}

	// Sets up the callback thread and returns its thread id
	int SetupCallbacks(void)
	{
		int thid = 0;

		thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
		if (thid >= 0)
		{
			sceKernelStartThread(thid, 0, 0);
		}

		return thid;
	}


	void initPlatform(const char* appName) {
		SetupCallbacks();
		scePowerSetClockFrequency(333, 333, 166);
		srand(time(NULL));
		vfpu_srand(time(NULL));

		Utilities::g_AppTimer.reset();
		Utilities::g_AppTimer.deltaTime();

		Utilities::detail::core_Logger = new Utilities::Logger("CORE");
		Utilities::detail::core_Logger->log("Stardust-Engine Initialized!");
		Utilities::detail::core_Logger->log("Platform Initialized!");

		Utilities::app_Logger = new Utilities::Logger(appName, std::string(appName) + "_log.log");
		Utilities::app_Logger->log("Application Start!");

		oslInitAudio();

	}

	void exitPlatform()
	{
		sceKernelDelayThread(1000000);
		oslDeinitAudio();
		delete Utilities::detail::core_Logger;
		delete Utilities::app_Logger;
		sceKernelExitGame();
	}

	void platformUpdate()
	{
		oslAudioVSync();
	}

#endif
}