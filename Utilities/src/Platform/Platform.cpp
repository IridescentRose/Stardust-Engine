#include "../../../Platform/include/Platform/Platform.h"
#include <Platform/Platform.h>
#include <Utilities/Timer.h>
#include <Utilities/Logger.h>
#include <Utilities/JSON.h>

#include <stdlib.h>
#include <time.h>

#ifndef STARDUST_UTILITIES_ONLY
#include <GFX/RenderCore.h>
#endif

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <Utilities/Input.h>

#include <psppower.h>
#include <pspkernel.h>
#include <pspmath.h>
#include <sound_utils/oslib.h>
#include <sound_utils/audio.h>
#include <intraFont.h>
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
#endif

	void exitPlatform()
	{
		delete Utilities::detail::core_Logger;
		delete Utilities::app_Logger;
#if CURRENT_PLATFORM == PLATFORM_PSP
		oslDeinitAudio();
		sceKernelExitGame();
#endif
	}

	void platformUpdate()
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		Utilities::updateInputs();
		oslAudioVSync();
#endif
#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
		PC::g_Window->update();
		if(PC::g_Window->shouldClose()){
			exitPlatform();
			exit(0);
		}
#endif
	}

	void initPlatform(const char* appName) {

#if CURRENT_PLATFORM == PLATFORM_PSP
		SetupCallbacks();
		scePowerSetClockFrequency(333, 333, 166);
		vfpu_srand(time(NULL));
		VirtualFileInit();
		oslInitAudio();
#endif
#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
		initPC();
#endif
		
		srand(time(NULL));

#ifndef STARDUST_UTILITIES_ONLY
		GFX::g_RenderCore = new GFX::RenderCore();
		GFX::g_RenderCore->init();
#endif

#if CURRENT_PLATFORM == PLATFORM_PSP
		intraFontInit();
#endif

		
		
		Utilities::g_AppTimer.reset();
		Utilities::g_AppTimer.deltaTime();

		Utilities::detail::core_Logger = new Utilities::Logger("CORE");
		Utilities::detail::core_Logger->log("Stardust-Engine Initialized!");
		Utilities::detail::core_Logger->log("Platform Initialized!");

		Utilities::app_Logger = new Utilities::Logger(appName, std::string(appName) + "_log.log");
		Utilities::app_Logger->log("Application Start!");

	}
}