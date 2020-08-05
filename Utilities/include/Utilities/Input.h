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

/* Printable keys */
#define GLFW_KEY_SPACE              -1
#define GLFW_KEY_APOSTROPHE         -1  /* ' */
#define GLFW_KEY_COMMA              -1  /* , */
#define GLFW_KEY_MINUS              -1  /* - */
#define GLFW_KEY_PERIOD             -1  /* . */
#define GLFW_KEY_SLASH              -1  /* / */
#define GLFW_KEY_0                  -1
#define GLFW_KEY_1                  -1
#define GLFW_KEY_2                  -1
#define GLFW_KEY_3                  -1
#define GLFW_KEY_4                  -1
#define GLFW_KEY_5                  -1
#define GLFW_KEY_6                  -1
#define GLFW_KEY_7                  -1
#define GLFW_KEY_8                  -1
#define GLFW_KEY_9                  -1
#define GLFW_KEY_SEMICOLON          -1  /* ; */
#define GLFW_KEY_EQUAL              -1  /* = */
#define GLFW_KEY_A                  -1
#define GLFW_KEY_B                  -1
#define GLFW_KEY_C                  -1
#define GLFW_KEY_D                  -1
#define GLFW_KEY_E                  -1
#define GLFW_KEY_F                  -1
#define GLFW_KEY_G                  -1
#define GLFW_KEY_H                  -1
#define GLFW_KEY_I                  -1
#define GLFW_KEY_J                  -1
#define GLFW_KEY_K                  -1
#define GLFW_KEY_L                  -1
#define GLFW_KEY_M                  -1
#define GLFW_KEY_N                  -1
#define GLFW_KEY_O                  -1
#define GLFW_KEY_P                  -1
#define GLFW_KEY_Q                  -1
#define GLFW_KEY_R                  -1
#define GLFW_KEY_S                  -1
#define GLFW_KEY_T                  -1
#define GLFW_KEY_U                  -1
#define GLFW_KEY_V                  -1
#define GLFW_KEY_W                  -1
#define GLFW_KEY_X                  -1
#define GLFW_KEY_Y                  -1
#define GLFW_KEY_Z                  -1
#define GLFW_KEY_LEFT_BRACKET       -1  /* [ */
#define GLFW_KEY_BACKSLASH          -1  /* \ */
#define GLFW_KEY_RIGHT_BRACKET      -1  /* ] */
#define GLFW_KEY_GRAVE_ACCENT       -1  /* ` */
#define GLFW_KEY_WORLD_1            -1 /* non-US #1 */
#define GLFW_KEY_WORLD_2            -1 /* non-US #2 */

/* Function keys */
#define GLFW_KEY_ESCAPE             -1
#define GLFW_KEY_ENTER              -1
#define GLFW_KEY_TAB                -1
#define GLFW_KEY_BACKSPACE          -1
#define GLFW_KEY_INSERT             -1
#define GLFW_KEY_DELETE             -1
#define GLFW_KEY_RIGHT              -1
#define GLFW_KEY_LEFT               -1
#define GLFW_KEY_DOWN               -1
#define GLFW_KEY_UP                 -1
#define GLFW_KEY_PAGE_UP            -1
#define GLFW_KEY_PAGE_DOWN          -1
#define GLFW_KEY_HOME               -1
#define GLFW_KEY_END                -1
#define GLFW_KEY_CAPS_LOCK          -1
#define GLFW_KEY_SCROLL_LOCK        -1
#define GLFW_KEY_NUM_LOCK           -1
#define GLFW_KEY_PRINT_SCREEN       -1
#define GLFW_KEY_PAUSE              -1
#define GLFW_KEY_F1                 -1
#define GLFW_KEY_F2                 -1
#define GLFW_KEY_F3                 -1
#define GLFW_KEY_F4                 -1
#define GLFW_KEY_F5                 -1
#define GLFW_KEY_F6                 -1
#define GLFW_KEY_F7                 -1
#define GLFW_KEY_F8                 -1
#define GLFW_KEY_F9                 -1
#define GLFW_KEY_F10                -1
#define GLFW_KEY_F11                -1
#define GLFW_KEY_F12                -1
#define GLFW_KEY_F13                -1
#define GLFW_KEY_F14                -1
#define GLFW_KEY_F15                -1
#define GLFW_KEY_F16                -1
#define GLFW_KEY_F17                -1
#define GLFW_KEY_F18                -1
#define GLFW_KEY_F19                -1
#define GLFW_KEY_F20                -1
#define GLFW_KEY_F21                -1
#define GLFW_KEY_F22                -1
#define GLFW_KEY_F23                -1
#define GLFW_KEY_F24                -1
#define GLFW_KEY_F25                -1
#define GLFW_KEY_KP_0               -1
#define GLFW_KEY_KP_1               -1
#define GLFW_KEY_KP_2               -1
#define GLFW_KEY_KP_3               -1
#define GLFW_KEY_KP_4               -1
#define GLFW_KEY_KP_5               -1
#define GLFW_KEY_KP_6               -1
#define GLFW_KEY_KP_7               -1
#define GLFW_KEY_KP_8               -1
#define GLFW_KEY_KP_9               -1
#define GLFW_KEY_KP_DECIMAL         -1
#define GLFW_KEY_KP_DIVIDE          -1
#define GLFW_KEY_KP_MULTIPLY        -1
#define GLFW_KEY_KP_SUBTRACT        -1
#define GLFW_KEY_KP_ADD             -1
#define GLFW_KEY_KP_ENTER           -1
#define GLFW_KEY_KP_EQUAL           -1
#define GLFW_KEY_LEFT_SHIFT         -1
#define GLFW_KEY_LEFT_CONTROL       -1
#define GLFW_KEY_LEFT_ALT           -1
#define GLFW_KEY_LEFT_SUPER         -1
#define GLFW_KEY_RIGHT_SHIFT        -1
#define GLFW_KEY_RIGHT_CONTROL      -1
#define GLFW_KEY_RIGHT_ALT          -1
#define GLFW_KEY_RIGHT_SUPER        -1
#define GLFW_KEY_MENU               -1

#else
#include <GLFW/glfw3.h>

//This is just for intercompat purposes to not error out
enum PspCtrlButtons
{
	/** Select button. */
	PSP_CTRL_SELECT = -1,
	/** Start button. */
	PSP_CTRL_START = -1,
	/** Up D-Pad button. */
	PSP_CTRL_UP = -1,
	/** Right D-Pad button. */
	PSP_CTRL_RIGHT = -1,
	/** Down D-Pad button. */
	PSP_CTRL_DOWN = -1,
	/** Left D-Pad button. */
	PSP_CTRL_LEFT = -1,
	/** Left trigger. */
	PSP_CTRL_LTRIGGER = -1,
	/** Right trigger. */
	PSP_CTRL_RTRIGGER = -1,
	/** Triangle button. */
	PSP_CTRL_TRIANGLE = -1,
	/** Circle button. */
	PSP_CTRL_CIRCLE = -1,
	/** Cross button. */
	PSP_CTRL_CROSS = -1,
	/** Square button. */
	PSP_CTRL_SQUARE = -1,
	/** Home button. In user mode this bit is set if the exit dialog is visible. */
	PSP_CTRL_HOME = -1,
	/** Hold button. */
	PSP_CTRL_HOLD = -1,
	/** Music Note button. */
	PSP_CTRL_NOTE = -1,
	/** Screen button. */
	PSP_CTRL_SCREEN = -1,
	/** Volume up button. */
	PSP_CTRL_VOLUP = -1,
	/** Volume down button. */
	PSP_CTRL_VOLDOWN = -1,
	/** Wlan switch up. */
	PSP_CTRL_WLAN_UP = -1,
	/** Remote hold position. */
	PSP_CTRL_REMOTE = -1,
	/** Disc present. */
	PSP_CTRL_DISC = -1,
	/** Memory stick present. */
	PSP_CTRL_MS = -1,
};

#define PSP_CTRL_ANALOG_X 0x3
#define PSP_CTRL_ANALOG_Y 0x5
#endif
#include <string>
#include <glm/glm.hpp>

/**
 * Small input utility which uses GLFW_Keys and PSP_CTRL buttons.
 * Both are accepted and they have default defines of -1 on the non-native platform
**/
namespace Stardust::Utilities {

	/**
	 * Is a key pressed down?
	**/
	bool KeyPressed(int key);

	/**
	 * Is a key held down?
	**/
	bool KeyHold(int key);

	/**
	 * Updates the input from the system-specific method
	**/
	void updateInputs();

	/*
		Returns from -1 -> 1 of the intensity of the selection.
		If it is not a joystick, it will always return 1
	*/
	float KeyStrength(int key);

	/**
	 * Adds an action bound to a specific key
	 * 
	 * @param action-- Action name
	 * @param key - The key number
	**/
	void addActionKeyPair(std::string action, int key);

	/**
	 * Deletes all action key pairs.
	**/
	void clearActionKeyPairs();

	/**
	 * Sets a specific action to a different key (rebind).
	 * 
	 * @param action - Action name to rebind
	 * @param key - New key
	**/
	void setActionKeyPair(std::string action, int key);

	/**
	 * Handles a bound action
	 * Bool - Held?
	 * Float - Strength
	**/
	typedef void (*ActionHandler)(bool, float);

	/**
	 * Adds a handler to an action upon a key press
	 * 
	 * @param action - Action name
	 * @param handler - The handler for the action
	**/
	void addActionHandler(std::string action, ActionHandler handler);

	/**
	 * Removes all handlers for a specified action.
	**/
	void clearActionHandlers();

	/**
	 * A method to get the Key name of PSP buttons.
	**/
	std::string toString(int but);

	/**
	 * Loads a .json configuration of action key pairs
	**/
	void LoadConfiguration(std::string path);

	
	/**
	 * Saves a .json configuration of action key pairs
	**/
	void SaveConfiguration(std::string path);

	/**
	 * Retrieves a key / button ID from an action
	 * 
	 * @param action - The ID to reverse lookup
	**/
	int buttonFromAction(std::string action);

	/**
	 * Waits for the next button press on PSP
	**/
	int nextAction();

	/**
	 * Gets the cursor position on PC, returns (-1, -1) on PSP
	**/
	glm::vec2 getCursorPos();
}