#pragma once

#define PLATFORM_PSP 	0
#define PLATFORM_WIN 	1
#define PLATFORM_NIX 	2
#define PLATFORM_VITA 	3

/*Anything else*/
#define PLATFORM_XXX 	-1

#ifndef CURRENT_PLATFORM
#error No Platform was Defined!
#endif

//Includes that should always be present for a specific platform
#if CURRENT_PLATFORM == PLATFORM_PSP
#include <pspkernel.h>
#include <pspdebug.h>
#include <string>
#include <sstream>
namespace std {
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}


#elif CURRENT_PLATFORM == PLATFORM_VITA
#include <vitasdk.h> 
// FIXME
typedef uint64_t u64;
typedef uint32_t u32;
#endif

#if CURRENT_PLATFORM == PLATFORM_WIN

#define WIN32_LEAN_AND_MEAN 1
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <Ws2tcpip.h>
#endif

#if CURRENT_PLATFORM != PLATFORM_PSP && CURRENT_PLATFORM != PLATFORM_VITA
#include <thread>
#endif

#if CURRENT_PLATFORM == PLATFORM_PSP 
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspnet.h>
#include <psputility.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
#include <psphttp.h>
#include <pspsdk.h>
#include <pspwlan.h>
#include <sound_utils/audio.h>
#endif

#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_NIX) || (CURRENT_PLATFORM == PLATFORM_VITA)
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <queue>
#include <netinet/tcp.h>
#include <fcntl.h>
#endif

#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
#include <SFML/Audio.hpp>
#include <Platform/PC/Window.h>
#endif
//Common includes - should be here for each platform
#include <chrono>
#include <stdexcept>

namespace Stardust::Platform {

	/*Note: These first three are not actually defined in Stardust-Platform - rather it is a prototype for Utilities / Core to fill out.*/
	/*
	Init Platform performs all needed platform initialization code
	*/
	void initPlatform(const char* appName = "APP");

	/*
	Exit Platform performs all needed platform exit code (this is for forced exits)
	*/
	void exitPlatform();

	/*
	Performs all per frame operations
	*/
	void platformUpdate();

	inline void delayForMS(int millis) {
#if CURRENT_PLATFORM == PLATFORM_PSP || CURRENT_PLATFORM == PLATFORM_VITA
		sceKernelDelayThread(millis * 1000);
#else
		std::this_thread::sleep_for(std::chrono::milliseconds(millis));
#endif
	}

	namespace detail{
		bool initNetworks();
		void closeNetworks();
		bool getHTTPFile(const char* url, const char* filepath);

		void closeSockets(int fd);
		bool setBlocking(int fd, int blocking);

#if CURRENT_PLATFORM == PLATFORM_PSP
typedef OSL_SOUND SoundEffect;
#else
typedef sf::Music SoundEffect;
#endif

		inline SoundEffect* loadSound(std::string path, bool streaming) {
#if CURRENT_PLATFORM == PLATFORM_PSP
			int i = 0;
			if (streaming) {
				i = OSL_FMT_STREAM;
			}
			else {
				i = OSL_FMT_NONE;
			}
			return oslLoadSoundFile(path.c_str(), i);
#else
			SoundEffect* m = new SoundEffect();
			m->openFromFile(path.c_str());
			return m;
#endif
		}

		inline void playSoundEffect(SoundEffect* effect, int channel) {

#if CURRENT_PLATFORM == PLATFORM_PSP
			oslPlaySound(effect, channel);
#else
			effect->play();
#endif

		}

		inline void pauseSoundEffect(SoundEffect* effect) {

#if CURRENT_PLATFORM == PLATFORM_PSP
			oslPauseSound(effect, -1);
#else
			effect->pause();
#endif

		}

		inline void stopSoundEffect(SoundEffect* effect) {

#if CURRENT_PLATFORM == PLATFORM_PSP
			oslStopSound(effect);
#else
			effect->stop();
#endif

		}

		inline void deleteSound(SoundEffect* effect) {
#if CURRENT_PLATFORM == PLATFORM_PSP
			oslDeleteSound(effect);
#else
			delete effect;
#endif
		}

		inline void setSoundLoop(SoundEffect* effect, bool loop) {
#if CURRENT_PLATFORM == PLATFORM_PSP
			oslSetSoundLoop(effect, loop);
#else
			effect->setLoop(loop);
#endif
		}


	}

#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
	inline void initPC(int width = 1280, int height = 720, std::string title = "Stardust Engine", bool fullScreen = false, bool vsync = false) {
		PC::g_Window = new PC::Window(width, height, title, fullScreen, vsync);
	}
#endif

}