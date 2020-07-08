#include <Graphics/2D/CharacterSprite.h>
#include <Utilities/Logger.h>

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

		defaultAnim = NULL;
		facing = CHARACTER_FACING_UP;
	}
	void CharacterSprite::setFacing(char direction)
	{
		facing = direction;

		switch (facing) {
		case CHARACTER_FACING_UP: {
			currentIndex = currentAnim->top.indexStart;
			indexEnd = currentAnim->top.animLength + currentIndex;
			break;
		}
		case CHARACTER_FACING_DOWN: {
			currentIndex = currentAnim->down.indexStart;
			indexEnd = currentAnim->down.animLength + currentIndex;
			break;
		}
		case CHARACTER_FACING_LEFT: {
			currentIndex = currentAnim->left.indexStart;
			indexEnd = currentAnim->left.animLength + currentIndex;
			break;
		}
		case CHARACTER_FACING_RIGHT: {
			currentIndex = currentAnim->right.indexStart;
			indexEnd = currentAnim->right.animLength + currentIndex;
			break;
		}
		default: {
			currentIndex = currentAnim->top.indexStart;
			indexEnd = currentAnim->top.animLength + currentIndex;
			break;
		}
		}
	}
	void CharacterSprite::tickPhase()
	{
		currentIndex++;

		if (currentIndex >= indexEnd) {
			currentAnim = defaultAnim;
			switch (facing) {
			case CHARACTER_FACING_UP: {
				currentIndex = currentAnim->top.indexStart;
				indexEnd = currentAnim->top.animLength + currentIndex;
				break;
			}
			case CHARACTER_FACING_DOWN: {
				currentIndex = currentAnim->down.indexStart;
				indexEnd = currentAnim->down.animLength + currentIndex;
				break;
			}
			case CHARACTER_FACING_LEFT: {
				currentIndex = currentAnim->left.indexStart;
				indexEnd = currentAnim->left.animLength + currentIndex;
				break;
			}
			case CHARACTER_FACING_RIGHT: {
				currentIndex = currentAnim->right.indexStart;
				indexEnd = currentAnim->right.animLength + currentIndex;
				break;
			}
			default: {
				currentIndex = currentAnim->top.indexStart;
				indexEnd = currentAnim->top.animLength + currentIndex;
				break;
			}
			}
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
		sceKernelDcacheWritebackInvalidateAll();

	}
	void CharacterSprite::addAnimEvent(std::string eventName, CharacterDirectionalAnimInfo* info)
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
			if (currentAnim != animationsInformation[eventName]) {
				currentAnim = animationsInformation[eventName];


				switch (facing) {
				case CHARACTER_FACING_UP: {
					currentIndex = currentAnim->top.indexStart;
					indexEnd = currentAnim->top.animLength + currentIndex;
					break;
				}
				case CHARACTER_FACING_DOWN: {
					currentIndex = currentAnim->down.indexStart;
					indexEnd = currentAnim->down.animLength + currentIndex;
					break;
				}
				case CHARACTER_FACING_LEFT: {
					currentIndex = currentAnim->left.indexStart;
					indexEnd = currentAnim->left.animLength + currentIndex;
					break;
				}
				case CHARACTER_FACING_RIGHT: {
					currentIndex = currentAnim->right.indexStart;
					indexEnd = currentAnim->right.animLength + currentIndex;
					break;
				}
				default: {
					currentIndex = currentAnim->top.indexStart;
					indexEnd = currentAnim->top.animLength + currentIndex;
					break;
				}
				}
			}
		}
		else {
			currentAnim = defaultAnim;

			switch (facing) {
			case CHARACTER_FACING_UP: {
				currentIndex = currentAnim->top.indexStart;
				indexEnd = currentAnim->top.animLength + currentIndex;
				break;
			}
			case CHARACTER_FACING_DOWN: {
				currentIndex = currentAnim->down.indexStart;
				indexEnd = currentAnim->down.animLength + currentIndex;
				break;
			}
			case CHARACTER_FACING_LEFT: {
				currentIndex = currentAnim->left.indexStart;
				indexEnd = currentAnim->left.animLength + currentIndex;
				break;
			}
			case CHARACTER_FACING_RIGHT: {
				currentIndex = currentAnim->right.indexStart;
				indexEnd = currentAnim->right.animLength + currentIndex;
				break;
			}
			default: {
				currentIndex = currentAnim->top.indexStart;
				indexEnd = currentAnim->top.animLength + currentIndex;
				break;
			}
			}
		}
	}
	void CharacterSprite::draw()
	{
		sceGumPushMatrix();
		sceGuEnable(GU_ALPHA_TEST);
		sceGuEnable(GU_BLEND);

		ScePspFVector3 scal = { scaleFactor.x, scaleFactor.y, 1.0f };
		sceGumScale(&scal);

		ScePspFVector3 loc = { offset.x, offset.y, layer };
		sceGumTranslate(&loc);

		sceGuEnable(GU_TEXTURE_2D);
		texRef->bindTexture(GU_NEAREST, GU_NEAREST, true);

		sceGuColor(rgba);
		std::array<float, 8> coords = characterSpriteAtlas->getTexture(currentIndex);
		vertices[0].u = coords[0];
		vertices[0].v = coords[1];
		vertices[1].u = coords[4];
		vertices[1].v = coords[5];

		sceGumDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 2, 0, vertices);

		sceGuDisable(GU_BLEND);
		sceGumPopMatrix();
	}
}