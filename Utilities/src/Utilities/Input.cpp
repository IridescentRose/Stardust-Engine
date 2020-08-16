#include <map>
#include <Utilities/Input.h>
#include <Utilities/Timer.h>
#include <Math/MathFuncs.h>
#include <json/json.h>
#include <fstream>
#include <iostream>

namespace Stardust::Utilities {

	std::map<std::string, int> mymap;
	std::map<std::string, ActionHandler> handles;

#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
	SceCtrlData oldPadData;
	SceCtrlData newPadData;

#if CURRENT_PLATFORM == PLATFORM_PSP
	unsigned char LeftX = newPadData.Lx;
	unsigned char LeftY = newPadData.Ly;
	// TODO
	unsigned char RightX = newPadData.Rx;
	unsigned char RightY = newPadData.Ry;
#else
	unsigned char LeftX = newPadData.lx;
	unsigned char LeftY = newPadData.ly;
	// TODO
	unsigned char RightX = newPadData.rx;
	unsigned char RightY = newPadData.ry;
#endif

	float getX()
	{
		return (((float)LeftX - 122.5f) / 122.5f); //Range of +/- 1.0f
	}

	float getY()
	{
		return (((float)LeftY - 122.5f) / 122.5f); //Range of +/- 1.0f
	}
#endif 

	void updateInputs()
	{

#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
		oldPadData = newPadData;
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceCtrlReadBufferPositive(&newPadData, 1);
#else
		sceCtrlReadBufferPositive(0, &newPadData, 1);
#endif
#endif
		//ON PC this is handled by Platform Update anyways.

		for (auto& [key, but] : mymap) {
			if (KeyHold(but) || KeyPressed(but)) {
				if(handles.find(key) != handles.end()){
					handles[key](KeyHold(but), KeyStrength(but));
				}
			}
		}

	}
	bool KeyPressed(int key)
	{

		if (key != -1) {
#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
		if (key == SCE_CTRL_ANALOG_X) {
			if (getX() > 0.3f || getX() < -0.3f) {
				return true;
			}
		}
		if (key == SCE_CTRL_ANALOG_Y) {
			if (getY() > 0.3f || getY() < -0.3f) {
				return true;
			}
		}

#if CURRENT_PLATFORM == PLATFORM_PSP
		unsigned int NewCtrlButtons = newPadData.Buttons;
		unsigned int OldCtrlButtons = oldPadData.Buttons;
#else
		unsigned int NewCtrlButtons = newPadData.buttons;
		unsigned int OldCtrlButtons = oldPadData.buttons;
#endif		

		if (NewCtrlButtons != OldCtrlButtons)
		{
			if (NewCtrlButtons & key)
			{
				return true;
			}
		}
		return false;
#else
		//Filter out phony SCE keys
			return glfwGetKey(Platform::PC::g_Window->getWindow(), key) == GLFW_PRESS;
#endif
		}
		return false;
	}
	bool KeyHold(int key)
	{
		if (key != -1) {
#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
		if (key == SCE_CTRL_ANALOG_X) {
			if (getX() > 0.3f || getX() < -0.3f) {
				return true;
			}
		}
		if (key == SCE_CTRL_ANALOG_Y) {
			if (getY() > 0.3f || getY() < -0.3f) {
				return true;
			}
		}

#if CURRENT_PLATFORM == PLATFORM_PSP
		unsigned int NewCtrlButtons = newPadData.Buttons;
#else
		unsigned int NewCtrlButtons = newPadData.buttons;
#endif	

		if (NewCtrlButtons & key)
		{
			return true;
		}

		return false;
#else
			return glfwGetKey(Platform::PC::g_Window->getWindow(), key) == GLFW_REPEAT;
		
#endif
		}
		return false;
	}
	float KeyStrength(int key)
	{

#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
		if (key == SCE_CTRL_ANALOG_X) {
			return getX();
		}
		if (key == SCE_CTRL_ANALOG_Y) {
			return getY();
		}
#endif

		if (KeyHold(key) || KeyPressed(key)) {
			return 1.0f;
		}
		return 0.0f;

		if (KeyHold(key) || KeyPressed(key)) {
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


#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
	std::string toString(int but)
	{
		switch(but){
		case SCE_CTRL_ANALOG_X:
			return "Analog X";

			break;

		case SCE_CTRL_ANALOG_Y:
			return "Analog Y";

			break;

		case SCE_CTRL_DOWN:
			return "Down";

			break;
		case SCE_CTRL_UP:
			return "Up";

			break;
		case SCE_CTRL_LEFT:
			return "Left";

			break;
		case SCE_CTRL_RIGHT:
			return "Right";

			break;


		case SCE_CTRL_CROSS:
			return "Cross";

			break;
		case SCE_CTRL_CIRCLE:
			return "Circle";

			break;
		case SCE_CTRL_TRIANGLE:
			return "Triangle";

			break;
		case SCE_CTRL_SQUARE:
			return "Square";

			break;


		case SCE_CTRL_RTRIGGER:
			return "RTrigger";

			break;
		case SCE_CTRL_LTRIGGER:
			return "LTrigger";

			break;
		case SCE_CTRL_SELECT:
			return "Select";

			break;
		case SCE_CTRL_START:
			return "Start";

			break;

	}

		return "Unbound";
	}
#endif
	void LoadConfiguration(std::string path)
	{
		mymap.clear();
		handles.clear();
		std::fstream file(path);

		Json::Value root;

		file >> root;
		file.close();

		Json::Value listen = root["listeners"];

		for (int i = 0; i < listen.size(); i++) {
			Json::Value temp = listen[i];
			std::string str = temp["name"].asString();
			int but = temp["button"].asInt();

			mymap.emplace(str, but);
		}
	}
	void SaveConfiguration(std::string path)
	{
		std::fstream file(path, std::ios::trunc);

		Json::Value v;

		Json::Value v2(Json::arrayValue);

		for (const auto& [str, but] : mymap) {
			Json::Value temp;
			temp["name"] = str.c_str();
			temp["button"] = but;

			v2.append(temp);
		}

		v["listeners"] = v2;

		file << v;
		file.close();
	}

	int buttonFromAction(std::string s){
		return mymap[s];
	}


#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
	int nextAction()
{

	Utilities::Timer t;
	t = Utilities::Timer();

	float delay = 0.1f;

	delay = 1.0f;
	while (delay > 0) {
		delay -= t.deltaTime();
		updateInputs();
	}

	while (true) {
		updateInputs();

		//check each button
		if (KeyHold(SCE_CTRL_UP)) {
			return SCE_CTRL_UP;
		}

		if (KeyHold(SCE_CTRL_DOWN)) {
			return SCE_CTRL_DOWN;
		}
		if (KeyHold(SCE_CTRL_LEFT)) {
			return SCE_CTRL_LEFT;
		}
		if (KeyHold(SCE_CTRL_RIGHT)) {
			return SCE_CTRL_RIGHT;
		}

		if (KeyHold(SCE_CTRL_TRIANGLE)) {
			return SCE_CTRL_TRIANGLE;
		}
		if (KeyHold(SCE_CTRL_CROSS)) {
			return SCE_CTRL_CROSS;
		}
		if (KeyHold(SCE_CTRL_SQUARE)) {
			return SCE_CTRL_SQUARE;
		}
		if (KeyHold(SCE_CTRL_CIRCLE)) {
			return SCE_CTRL_CIRCLE;
		}

		if (KeyHold(SCE_CTRL_LTRIGGER)) {
			return SCE_CTRL_LTRIGGER;
		}
		if (KeyHold(SCE_CTRL_RTRIGGER)) {
			return SCE_CTRL_RTRIGGER;
		}
		if (KeyHold(SCE_CTRL_SELECT)) {
			return SCE_CTRL_SELECT;
		}
		if (KeyHold(SCE_CTRL_START)) {
			return SCE_CTRL_START;
		}

		if (getX() >= 0.5f) {
			return (int)SCE_CTRL_ANALOG_X;
		}
		if (getY() >= 0.5f) {
			return (int)SCE_CTRL_ANALOG_Y;
		}
		if (getX() <= -0.5f) {
			return (int)SCE_CTRL_ANALOG_X;
		}
		if (getY() <= -0.5f) {
			return (int)SCE_CTRL_ANALOG_Y;
		}
	}

	return (int)0;
}
#endif
	glm::vec2 getCursorPos()
	{
#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_VITA)
		return glm::vec2(-1, -1);
#else

		double curX, curY;
		curX = curY = -1.0;

		glfwGetCursorPos(Platform::PC::g_Window->getWindow(), &curX, &curY);

		return glm::vec2((float)curX, (float)curY);
#endif
	}
}