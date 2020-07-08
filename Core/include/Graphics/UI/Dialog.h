#pragma once
#include <Graphics/2D/SpriteBase.h>
#include "MarqueeText.h"
#include <Audio/AudioClip.h>
#include <Events/Events.h>

namespace Stardust::Graphics::UI {
	enum InteractionType {
		INTERACTION_TYPE_NONE,
		INTERACTION_TYPE_SELECTION
	};

	struct Dialog {
		std::string text;
		char interactionType;
		std::vector<std::string> choice;
		Events::EventHandler e;
	};


	class Dialogue {
	public:
		Dialogue(Render2D::Sprite* sprt);

		void reset();
		void setDialogue(Dialog* d);

		void show();
		void hide();

		void update();
		void draw();

		bool isEngaged();

		MarqueeText* main;
		UIText* helper;
		Render2D::Sprite* dialogueBox;
		Audio::AudioClip* sel;
	private:
		bool display;
		int selIndex;
		int selPos;
		int selFrame;
		int exitIndex;

		Dialog* info;
	};

	class DialogStack {
	public:

		DialogStack(Dialogue* d, int textWrap = 52, std::string usr = "default");

		void addDialog(Dialog* d);
		void clearDialog();

		void update();

	private:
		Dialogue* dial;
		std::string username;
		std::queue<Dialog*> dialogs;
		int wrap;

	};
}