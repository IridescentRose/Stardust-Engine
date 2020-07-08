#include <Graphics/2D/AnimTickController.h>

namespace Stardust::Graphics::Render2D {
	AnimationTickController::AnimationTickController()
	{
		charSprites.clear();
		spriteAnims.clear();
		tileMaps.clear();

		count = 0;
		rate = 20;
		charRate = 20;

	}
	AnimationTickController::~AnimationTickController()
	{
		//thr->Kill();
	}
	void AnimationTickController::setTickRate(int ticksPerSecond)
	{
		rate = ticksPerSecond;
	}
	void AnimationTickController::setCharacterTickRateRelative(int relative)
	{
		charRate = relative;
	}
	void AnimationTickController::addAnimatedCharacter(CharacterSprite* charSprite)
	{
		charSprites.push_back(charSprite);
	}
	void AnimationTickController::addAnimatedObject(SpriteAnimated* obj)
	{
		spriteAnims.push_back(obj);
	}
	void AnimationTickController::addAnimatedObject(TilemapAnim* obj)
	{
		tileMaps.push_back(obj);
	}

	void AnimationTickController::clearAnimatedObjects()
	{
		tileMaps.clear();
		spriteAnims.clear();
	}
	void AnimationTickController::clearCharacters()
	{
		charSprites.clear();
	}


	void AnimationTickController::tick()
	{
		for (auto obj : charSprites) {
			if (count % charRate == 0) {
				obj->tickPhase();
			}
		}
		for (auto obj : spriteAnims) {
			if (count % rate == 0) {
				obj->tickPhase();
			}
		}
		for (auto obj : tileMaps) {
			if (count % rate == 0) {
				obj->tickPhase();
			}
		}
		count++;
	}
}