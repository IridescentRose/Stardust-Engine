#include <map>
#include <Utilities/Input.h>
#include <Math/MathFuncs.h>

namespace Stardust::Utilities {
	SceCtrlData oldPadData;
	SceCtrlData newPadData;
	std::map<std::string, int> mymap;
	std::map<std::string, ActionHandler> handles;


	void updateInputs()
	{
		oldPadData = newPadData;
		sceCtrlReadBufferPositive(&newPadData, 1);

		for (auto& [key, but] : mymap) {
			if (KeyHold(but) || KeyPressed(but)) {
				handles[key](KeyHold(but), KeyStrength(but));
			}
		}

	}
	bool KeyPressed(int key)
	{
		if (key == PSP_CTRL_ANALOG_X) {
			if (abs(((float)newPadData.Lx - 122.5f) / 122.5f) > 0) {
				return true;
			}
		}
		if (key == PSP_CTRL_ANALOG_X) {
			if (abs(((float)newPadData.Ly - 122.5f) / 122.5f) > 0) {
				return true;
			}
		}

		if (newPadData.Buttons != oldPadData.Buttons)
		{
			if (newPadData.Buttons & key)
			{
				return true;
			}
		}

		return false;
	}
	bool KeyHold(int key)
	{
		if (key == PSP_CTRL_ANALOG_X) {
			if (abs(((float)newPadData.Lx - 122.5f) / 122.5f) > 0) {
				return true;
			}
		}
		if (key == PSP_CTRL_ANALOG_Y) {
			if (abs(((float)newPadData.Ly - 122.5f) / 122.5f) > 0) {
				return true;
			}
		}

		if (newPadData.Buttons & key)
		{
			return true;
		}

		return false;
	}
	float KeyStrength(int key)
	{
		if (key == PSP_CTRL_ANALOG_X) {
			return ((float)newPadData.Lx - 122.5f) / 122.5f;
		}
		if (key == PSP_CTRL_ANALOG_Y) {
			return ((float)newPadData.Ly - 122.5f) / 122.5f;
		}

		if (KeyHold(key)) {
			return 1.0f;
		}
		return 0.0f;
	}


	void addActionKeyPair(std::string action, int key)
	{
		mymap.emplace(action, key);
	}
	
	void clearActionKeyPairs()
	{
		mymap.clear();
	}

	void setActionKeyPair(std::string action, int key)
	{
		mymap[action] = key;
	}
	
	void addActionHandler(std::string action, ActionHandler handler)
	{
		handles.emplace(action, handler);
	}

	void clearActionHandlers()
	{
		handles.clear();
	}
}