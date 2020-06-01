#include <Graphics/UI/UISystem.h>

namespace Stardust::Graphics::UI {
	UISystem::UISystem()
	{
	}
	UISystem::~UISystem()
	{
	}

	void UISystem::parseUI(std::string path)
	{
		Json::Value v = Utilities::JSON::openJSON(path);

		sel = v["buttonSelected"].asInt();

		for (int i = 0; i < v["elements"].size(); i++) {
			Json::Value v2 = v["elements"][i];
			std::string type = v2["type"].asString();

			if (type == "button") {
				parseButton(v2);
			}
			else if (type == "text") {
				parseText(v2);
			}
		}
	}

	void UISystem::update()
	{

	}

	void UISystem::draw()
	{
		for (auto& [idx, but] : buttons) {
			but->draw();
		}
		for (auto& [str, txt] : text) {
			txt->draw();
		}
	}
	
	void UISystem::parseButton(Json::Value v)
	{
		std::string id = v["id"].asString();
		int idx = v["index"].asInt();

		glm::vec2 pos = { v["position"]["x"].asInt(), v["position"]["y"].asInt() };

		UIText* txt = new UIText(pos, v["content"].asString());

		std::string intrafontOption = v["options"]["align"].asString();

		int opt = intrafontOption == "left" ? INTRAFONT_ALIGN_LEFT : intrafontOption == "center" ? INTRAFONT_ALIGN_CENTER : intrafontOption == "right" ? INTRAFONT_ALIGN_RIGHT : INTRAFONT_ALIGN_FULL;
		txt->setOptions({ v["options"]["size"].asFloat(), GU_RGBA(v["options"]["color"]["r"].asInt(), v["options"]["color"]["g"].asInt(), v["options"]["color"]["b"].asInt(), v["options"]["color"]["a"].asInt()), opt });

		glm::vec2 size = { v["size"]["x"].asInt(), v["size"]["y"].asInt() };

		UIButton* button = new UIButton(pos, size, TextureUtil::LoadPng(v["imageUnsel"].asString()), TextureUtil::LoadPng(v["imageSel"].asString()), txt);

		buttons.emplace(idx, button);
		buttonS.emplace(id, button);
	}

	void UISystem::parseText(Json::Value v)
	{
		std::string id = v["id"].asString();
		UIText* txt = new UIText({v["position"]["x"].asInt(), v["position"]["y"].asInt() }, v["content"].asString());

		std::string intrafontOption = v["options"]["align"].asString();

		int opt = intrafontOption == "left" ? INTRAFONT_ALIGN_LEFT : intrafontOption == "center" ? INTRAFONT_ALIGN_CENTER : intrafontOption == "right" ? INTRAFONT_ALIGN_RIGHT : INTRAFONT_ALIGN_FULL;
		txt->setOptions({v["options"]["size"].asFloat(), GU_RGBA(v["options"]["color"]["r"].asInt(), v["options"]["color"]["g"].asInt(), v["options"]["color"]["b"].asInt(), v["options"]["color"]["a"].asInt()), opt});

		text.emplace(id, txt);
	}
}