#include "../../../include/Engines/VN/VisualNovelEngine.h"

namespace Stardust::Engines::VN {
	VisualNovelEngine::VisualNovelEngine(MarqueeText* mq, Dialogue* dialog, DialogStack* dialogStack, std::string vnFile)
	{
		dialogue = dialog;
		dialogue->main = mq;
		stack = dialogStack;
		sceneMap.clear();


		parseVN(vnFile);
		pc = 0;
	}
	VisualNovelEngine* g_VNE;
	VisualNovelEngine::~VisualNovelEngine()
	{
	}

	void VisualNovelEngine::update()
	{

		dialogue->update();
		stack->update();

		if (!dialogue->isEngaged()) {
			parseCommand(currentScript->script[pc++]);
		}
	}

	void VisualNovelEngine::draw()
	{

		sceGuEnable(GU_BLEND);
		if (sceneMap[currScene]->sprite) {
			sceneMap[currScene]->sprt->SetPosition(240, 136);
			sceneMap[currScene]->sprt->Draw();
		}
		else {
			sceGuClearColor(sceneMap[currScene]->clearColor);
		}
		for (auto& [str, character] : characterMap) {
			if (character->hasImage && !character->hidden) {
				character->states[character->currentIDX]->SetPosition(character->position.x, character->position.y);
				character->states[character->currentIDX]->Draw();
			}
		}

		for (auto& [str, image] : imageMap) {
			if (!image->hidden) {
				image->sprt->SetPosition(image->position.x, image->position.y);
				image->sprt->Draw();
			}
		}

		dialogue->draw();
	}

	int VisualNovelEngine::eventHandler(Events::Event* e)
	{
		int chose = e->event_type;

		g_VNE->parseCommand(g_VNE->commands[chose]);

		return 0;
	}

	void VisualNovelEngine::parseCommand(Json::Value v)
	{
		std::string cmd = v["cmd"].asString();

		Utilities::app_Logger->log("Parsing " + cmd);

		if (cmd == "scene") {
			std::string nextScene = v["sceneName"].asString();
			currScene = nextScene;
		}
		else if (cmd == "sound_play") {
			std::string sound = v["soundName"].asString();
			soundMap[sound]->clip->Play(soundMap[sound]->channel);
		}
		else if (cmd == "message") {
			std::string characterID = v["character"].asString();
			Dialog* d = new Dialog();
			d->interactionType = INTERACTION_TYPE_NONE;
			d->text = "";

			Utilities::app_Logger->log(characterID);
			if (characterID != "none") {
				Utilities::app_Logger->log("MSG");
				d->text += characterMap[characterID]->name;
				d->text += ": ";
			}

			Utilities::app_Logger->log("MSG");
			d->text += v["message"].asString();

			stack->addDialog(d);
		}
		else if (cmd == "sound_stop") {
			std::string sound = v["soundName"].asString();
			if (sound == "all") {
				for (auto& [str, clip] : soundMap) {
					clip->clip->Stop();
				}
			}
			else {
				soundMap[sound]->clip->Stop();
			}
		}
		else if (cmd == "show_char") {
			std::string character = v["character"].asString();
			characterMap[character]->hidden = false;
			characterMap[character]->position = { v["position"]["x"].asInt(),v["position"]["y"].asInt() };
		}
		else if (cmd == "hide_char") {
			std::string character = v["character"].asString();
			characterMap[character]->hidden = true;
		}
		else if (cmd == "show_img") {
			std::string image = v["image"].asString();
			imageMap[image]->hidden = false;
			imageMap[image]->position = { v["position"]["x"].asInt(),v["position"]["y"].asInt() };
		}
		else if (cmd == "hide_img") {
			std::string image = v["image"].asString();
			imageMap[image]->hidden = true;
		}
		else if (cmd == "jump") {
			std::string nextScript = v["scriptName"].asString();
			currentScript = scriptMap[nextScript];
			pc = 0;
		}
		else if (cmd == "menu") {
			std::string prompt = v["prompt"].asString();

			Dialog* d = new Dialog();
			d->interactionType = INTERACTION_TYPE_SELECTION;
			d->text = prompt;
			d->choice.clear();
			d->e = eventHandler;

			commands.clear();

			for (int i = 0; i < v["options"].size(); i++) {
				commands.push_back(v["options"][i]["action"]);
				d->choice.push_back(v["options"][i]["name"].asString());
			}

			stack->addDialog(d);
		}
		else if (cmd == "exit") {
			sceKernelExitGame();
		}
		
	}
	void VisualNovelEngine::parseVN(std::string file)
	{
		Json::Value v = Utilities::JSON::openJSON(file);

		//FIRST PARSE INFORMATION STRUCTURE
		Json::Value info = v["information"];

		Utilities::app_Logger->log("Parsing info");


		for (int i = 0; i < v["information"]["scenes"].size();i++) {
			parseScene(v["information"]["scenes"][i]);
		}

		for (auto [str, sce] : sceneMap) {
			Utilities::app_Logger->log(str);
		}

		currScene = "scene1";
		for (int i = 0; i < v["information"]["characters"].size(); i++) {
			parseCharacter(v["information"]["characters"][i]);
		}

		for (int i = 0; i < v["information"]["images"].size(); i++) {
			parseImage(v["information"]["images"][i]);
		}

		for (int i = 0; i < v["information"]["sounds"].size(); i++) {
			parseSound(v["information"]["sounds"][i]);
		}

		for (int i = 0; i < v["scripts"].size(); i++) {
			parseScript(v["scripts"][i]);
		}

		std::string beginScript = v["information"]["beginAt"].asString();

		currentScript = scriptMap[beginScript];
		pc = 0;
	}

	void VisualNovelEngine::parseScene(Json::Value v)
	{
		VNScene* newScene = new VNScene();

		std::string name = v["name"].asString();

		std::string fileName = v["imageFile"].asString();

		newScene->sprite = (fileName != "none");
		Utilities::app_Logger->log(name + " " + std::to_string(newScene->sprite));

		if (newScene->sprite) {
			newScene->sprt = new Sprite(Graphics::TextureUtil::LoadPng(fileName));
		}
		else {
			newScene->clearColor = GU_RGBA(v["clearColor"]["r"].asInt(), v["clearColor"]["g"].asInt(), v["clearColor"]["b"].asInt(), v["clearColor"]["a"].asInt());
		}

		sceneMap.emplace(name, newScene);
	}

	void VisualNovelEngine::parseCharacter(Json::Value v)
	{
		VNCharacter* newChar = new VNCharacter();


		std::string id = v["id"].asString();
		newChar->name = v["name"].asString();
		newChar->multiples = v["multiple"].asBool();

		if (!newChar->multiples) {
			std::string file = v["imageFile"].asString();
			newChar->hasImage = file != "none";
			if (newChar->hasImage) {
				newChar->currentIDX = "default";
				newChar->states.emplace("default", new Sprite(Graphics::TextureUtil::LoadPng(file)));
			}
		}
		else {
			newChar->hasImage = true;
			newChar->currentIDX = v["imageFiles"][0]["state"].asString();
			for (int i = 0; i < v["imageFiles"].size(); i++) {
				std::string id = v["imageFiles"][i]["state"].asString();
				std::string file = v["imageFiles"][i]["imageFile"].asString();
				newChar->states.emplace(id, new Sprite(Graphics::TextureUtil::LoadPng(file)));
			}
		}
		newChar->hidden = true;

		newChar->position = { 240, 136 };
		characterMap.emplace(id, newChar);
	}

	void VisualNovelEngine::parseImage(Json::Value v)
	{
		VNImage* newImage = new VNImage();
		std::string id = v["id"].asString();

		newImage->sprt = new Sprite(Graphics::TextureUtil::LoadPng(v["imageFile"].asString()));

		newImage->position = { 240, 136 };
		newImage->hidden = true;

		imageMap.emplace(id, newImage);
	}

	void VisualNovelEngine::parseSound(Json::Value v)
	{
		VNSound* newSound = new VNSound();
		std::string id = v["name"].asString();
		newSound->loop = v["loop"].asBool();
		newSound->channel = v["channel"].asInt();

		newSound->clip = new Audio::AudioClip(v["soundFile"].asString());
		//newSound->clip->SetLoop(newSound->loop);

		soundMap.emplace(id, newSound);
	}

	void VisualNovelEngine::parseScript(Json::Value v)
	{
		VNScript* newScript = new VNScript();
		std::string id = v["name"].asString();
		Utilities::app_Logger->log(id);

		newScript->script = v["actions"];

		scriptMap.emplace(id, newScript);
	}

}

