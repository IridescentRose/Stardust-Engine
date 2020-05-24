#pragma once
#include <Graphics/UI/Dialog.h>
#include <Utilities/Logger.h>

namespace Stardust::Engines::VN {
	using namespace Graphics::UI;
	using namespace Graphics::Render2D;

	struct VNScene {
		bool sprite;
		Sprite* sprt;
		unsigned int clearColor;
	};

	struct VNImage {
		Sprite* sprt;
		glm::vec2 position;
		bool hidden;
	};

	struct VNSound {
		bool loop;
		int channel;
		Audio::AudioClip* clip;
	};

	struct VNCharacter {
		std::string name;
		bool hasImage;
		bool multiples;
		std::string currentIDX;
		std::map<std::string, Sprite*> states;
		glm::vec2 position;
		bool hidden;
	};

	struct VNScript {
		Json::Value script;
	};

	class VisualNovelEngine {
	public:
		VisualNovelEngine(MarqueeText* mq, Dialogue* dialog, DialogStack* dialogStack, std::string vnFile);
		~VisualNovelEngine();

		void update();
		void draw();

		static int eventHandler(Events::Event*);

		std::vector<Json::Value> commands;

	private:
		Dialogue* dialogue;
		DialogStack* stack;

		void parseCommand(Json::Value v);
		void parseVN(std::string file);
		void parseScene(Json::Value v);
		void parseCharacter(Json::Value v);
		void parseImage(Json::Value v);
		void parseSound(Json::Value v);
		void parseScript(Json::Value v);


		std::string currScene;
		VNScript* currentScript;
		int pc;

		std::map<std::string, VNImage*> imageMap;
		std::map<std::string, VNScene*> sceneMap;
		std::map<std::string, VNSound*> soundMap;
		std::map<std::string, VNCharacter*> characterMap;
		std::map<std::string, VNScript*> scriptMap;
	};

	extern VisualNovelEngine* g_VNE;
}