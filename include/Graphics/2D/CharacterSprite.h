#pragma once
#include "SpriteAdvanced.h"
#include <Graphics/TextureUtil.h>
#include <Math/Vector.h>
#include <string>
#include <map>

namespace Stardust::Graphics::Render2D {

	enum CharacterFacingDirection {
		CHARACTER_FACING_UP,
		CHARACTER_FACING_DOWN,
		CHARACTER_FACING_LEFT,
		CHARACTER_FACING_RIGHT,
	};

	struct CharacterAnimInfo {
		short indexStart;
		short animLength;
	};

	struct CharacterDirectionalAnimInfo {
		CharacterAnimInfo top;
		CharacterAnimInfo down;
		CharacterAnimInfo left;
		CharacterAnimInfo right;
	};

	class CharacterSprite : public Sprite2{
	public:
		CharacterSprite();
		CharacterSprite(glm::vec2 characterSize, TextureAtlas* atlas, Texture* texture);

		void setFacing(char direction);

		void tickPhase();
		void setPhase(int i);

		void addAnimEvent(std::string eventName, CharacterDirectionalAnimInfo* info);
		void clearAnimEvents();

		void setDefaultAnimEvent(std::string eventName);
		void triggerAnimEvent(std::string name);

		void draw();

		inline char getFacing() {
			return facing;
		}

	private:
		std::map<std::string, CharacterDirectionalAnimInfo*> animationsInformation;
		short currentIndex;
		short indexEnd;
		TextureAtlas* characterSpriteAtlas;
		CharacterDirectionalAnimInfo* defaultAnim;
		CharacterDirectionalAnimInfo* currentAnim;

		char facing;
	};
}