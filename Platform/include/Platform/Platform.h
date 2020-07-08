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
#elif CURRENT_PLATFORM == PLATFORM_VITA
#include <vitasdk.h> 
// FIXME
typedef uint64_t u64;
typedef uint32_t u32;
#endif

/*Note: This is not actually defined in Stardust-Platform - rather it is a prototype for Utilities / Core to fill out.*/
namespace Stardust::Platform {
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
}