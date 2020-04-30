#pragma once
#include <Math/Vector.h>
#include "TilemapAnim.h"
#include <Graphics/TextureUtil.h>

namespace Stardust::Graphics::Render2D {
	class CharacterSprite {
	public:
		CharacterSprite();
		CharacterSprite(glm::vec2 characterSize, TextureAtlas* atlas, Texture* texture);

		
	private:

	};
}