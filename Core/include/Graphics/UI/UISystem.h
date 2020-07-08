#pragma once
#include "UIButton.h"
#include "UIText.h"
#include "MarqueeText.h"
#include "Dialog.h"

namespace Stardust::Graphics::UI {
	class UISystem {
	public:
		UISystem();
		~UISystem();

		void parseUI(std::string path);

		void update();
		void draw();

		int sel = 0;
		std::map<int, UIButton*> buttons;
		std::map<std::string, UIButton*> buttonS;
		std::map<std::string, UIText*> text;

	private:

		void parseButton(Json::Value v);
		void parseText(Json::Value v);

	};
}