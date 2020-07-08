#pragma once
#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <malloc.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <psputility.h>
#include <string.h>

#define PSP_CTRL_ANALOG_X 0x3
#define PSP_CTRL_ANALOG_Y 0x5

#endif
#include <string>

namespace Stardust::Utilities {

	void updateInputs();

	bool KeyPressed(int key);
	bool KeyHold(int key);

	/*
		Returns from -1 -> 1 of the intensity of the selection.
		If it is not a joystick, it will always return 1
	*/
	float KeyStrength(int key);

	void addActionKeyPair(std::string action, int key);
	void clearActionKeyPairs();

	void setActionKeyPair(std::string action, int key);


	/**
	* Bool - Held?
	* Float - Strength
	*/
	typedef void (*ActionHandler)(bool, float);

	void addActionHandler(std::string action, ActionHandler handler);
	void clearActionHandlers();

	std::string toString(int but);

	void LoadConfiguration(std::string path);
	void SaveConfiguration(std::string path);

	int buttonFromAction(std::string);

	int nextAction();
}