#pragma once
#include "CharacterSprite.h"
#include "SpriteAnimated.h"
#include "TilemapAnim.h"
#include <Utilities/Thread.h>

namespace Stardust::Graphics::Render2D {

	class AnimationTickController {
	public:
		AnimationTickController();
		~AnimationTickController();

		void setTickRate(int framesPerTick);
		void setCharacterTickRateRelative(int relative);

		void addAnimatedCharacter(CharacterSprite* charSprite);
		void addAnimatedObject(SpriteAnimated* obj);
		void addAnimatedObject(TilemapAnim* obj);

		void clearAnimatedObjects();
		void clearCharacters();
		void tick();

	private:


		std::vector<CharacterSprite*> charSprites;
		std::vector<SpriteAnimated*> spriteAnims;
		std::vector<TilemapAnim*> tileMaps;
		Utilities::Thread* thr;

		volatile int rate;
		volatile int charRate;
	};
}