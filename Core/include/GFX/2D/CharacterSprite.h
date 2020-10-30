#pragma once
#include "Sprite.h"

namespace Stardust::GFX::Render2D {
	struct CharacterAnimInfo {
		short indexStart;
		short animLength;
	};
	
	class CharacterSprite : public Sprite{
	public:
		CharacterSprite(glm::vec2 characterSize, TextureAtlas* atlas, unsigned int texture);

		void tickPhase();
		void setPhase(int i);

		void addAnimEvent(std::string eventName, CharacterAnimInfo* info);
		void clearAnimEvents();

		void triggerAnimEvent(std::string name);

		void draw();

	private:
		std::map<std::string, CharacterAnimInfo*> animationsInformation;
		short currentIndex;
		short indexEnd;
		TextureAtlas* characterSpriteAtlas;
		CharacterAnimInfo* currentAnim;
	};
}