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
#endif

#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_NIX) || (CURRENT_PLATFORM == PLATFORM_VITA)
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <queue>
#include <fcntl.h>
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
	}
}