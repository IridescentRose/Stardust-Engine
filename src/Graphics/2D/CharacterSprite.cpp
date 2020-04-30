#include <Graphics/2D/CharacterSprite.h>

namespace Stardust::Graphics::Render2D {
	CharacterSprite::CharacterSprite()
	{
		animationsInformation.clear();
		currentIndex = 0;
	}
	CharacterSprite::CharacterSprite(glm::vec2 characterSize, TextureAtlas* atlas, Texture* texture) : Sprite2(texture, characterSize)
	{
		animationsInformation.clear();
		currentIndex = 0;
		characterSpriteAtlas = atlas;

		std::array<float, 8> coords = atlas->getTexture(0);

		vertices[0].u = coords[0];
		vertices[0].v = coords[1];
		vertices[1].u = coords[4];
		vertices[1].v = coords[5];
		CharacterAnimInfo* anim = new CharacterAnimInfo();
		anim->animLength = 0;
		anim->indexStart = 0;

		defaultAnim = anim;
	}
	void CharacterSprite::tickPhase()
	{
		currentIndex++;

		if (currentIndex >= currentAnim->indexStart + currentAnim->animLength) {
			currentAnim = defaultAnim;
			currentIndex = defaultAnim->indexStart;
		}

		setPhase(currentIndex);
	}
	void CharacterSprite::setPhase(int i)
	{
		std::array<float, 8> coords = characterSpriteAtlas->getTexture(currentIndex);
		vertices[0].u = coords[0];
		vertices[0].v = coords[1];
		vertices[1].u = coords[4];
		vertices[1].v = coords[5];
	}
	void CharacterSprite::addAnimEvent(std::string eventName, CharacterAnimInfo* info)
	{
		animationsInformation.emplace(eventName, info);
	}
	
	void CharacterSprite::clearAnimEvents()
	{
		for (auto [key, val] : animationsInformation) {
			delete val;
		}
		animationsInformation.clear();
	}

	void CharacterSprite::setDefaultAnimEvent(std::string eventName)
	{
		if (animationsInformation.find(eventName) != animationsInformation.end()) {
			defaultAnim = animationsInformation[eventName];
		}
	}

	void CharacterSprite::triggerAnimEvent(std::string eventName)
	{
		if (animationsInformation.find(eventName) != animationsInformation.end()) {
			currentAnim = animationsInformation[eventName];
			currentIndex = currentAnim->indexStart;
		}
		else {
			currentAnim = defaultAnim;
			currentIndex = defaultAnim->indexStart;
		}
	}
}