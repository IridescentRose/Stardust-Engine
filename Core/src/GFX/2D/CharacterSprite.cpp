#include <GFX/2D/CharacterSprite.h>

namespace Stardust::GFX::Render2D {

	CharacterSprite::CharacterSprite(glm::vec2 characterSize, TextureAtlas* atlas, unsigned int texture) : Sprite(texture, characterSize) {
		animationsInformation.clear();
		currentIndex = 0;
		indexEnd = 0;
		characterSpriteAtlas = atlas;

		setPhase(0);

		CharacterAnimInfo* anim = new CharacterAnimInfo();
		anim->animLength = 0;
		anim->indexStart = 0;

		currentAnim = anim;
	}

	void CharacterSprite::tickPhase(){
		currentIndex++;

		if (currentIndex >= indexEnd){
			currentIndex = currentAnim->indexStart;
		}

		setPhase(currentIndex);
	}
	
	void CharacterSprite::setPhase(int i){
		std::array<float, 8> coords = characterSpriteAtlas->getTexture(i);

		mesh.uv = {
			coords[0], coords[1],
			coords[2], coords[3],
			coords[4], coords[5],
			coords[6], coords[7],
		};
		model.addData(mesh);
	}

	void CharacterSprite::addAnimEvent(std::string eventName, CharacterAnimInfo* info){
		animationsInformation.emplace(eventName, info);
	}
	
	void CharacterSprite::clearAnimEvents(){
		for (auto [key, val] : animationsInformation) {
			delete val;
		}
		animationsInformation.clear();
	}

	void CharacterSprite::triggerAnimEvent(std::string name){
		if (animationsInformation.find(name) != animationsInformation.end()) {
			if (currentAnim != animationsInformation[name]) {
				currentAnim = animationsInformation[name];

				currentIndex = currentAnim->indexStart;
				indexEnd = currentAnim->animLength + currentIndex;
			}
		}
	}

	void CharacterSprite::draw() {
		GFX::pushMatrix();
		GFX::clearModelMatrix();
		GFX::translateModelMatrix(glm::vec3(offset.x, offset.y, 1.0f));
		GFX::pushMatrix();
		GFX::scaleModelMatrix(glm::vec3(scaleFactor.x, scaleFactor.y, 1.0f));
		GFX::pushMatrix();
		GFX::rotateModelMatrix({ rotation.x, rotation.y, 0.0f });


		g_TextureManager->bindTex(tex);
		model.draw();

		GFX::popMatrix();
		GFX::popMatrix();
		GFX::popMatrix();
	}
}