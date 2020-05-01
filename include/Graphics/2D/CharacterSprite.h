#pragma once
#include "SpriteAdvanced.h"
#include <Graphics/TextureUtil.h>
#include <Math/Vector.h>
#include <string>
#include <map>

namespace Stardust::Graphics::Render2D {

	struct CharacterAnimInfo {
		short indexStart;
		short animLength;
	};

	class CharacterSprite : public Sprite2{
	public:
		CharacterSprite();
		CharacterSprite(glm::vec2 characterSize, TextureAtlas* atlas, Texture* texture);

		void tickPhase();
		void setPhase(int i);

		void addAnimEvent(std::string eventName, CharacterAnimInfo* info);
		void clearAnimEvents();

		void setDefaultAnimEvent(std::string eventName);
		void triggerAnimEvent(std::string name);

		void draw();

	private:
		std::map<std::string, CharacterAnimInfo*> animationsInformation;
		short currentIndex;
		TextureAtlas* characterSpriteAtlas;
		CharacterAnimInfo* defaultAnim;
		CharacterAnimInfo* currentAnim;
	};
}