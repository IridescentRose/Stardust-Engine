#pragma once
#include "CharacterSprite.h"
#include <Core/Controller.h>
#include "AnimTickController.h"

namespace Stardust::Graphics::Render2D {
	class Controller2D : public Core::ControllerBase {
	public:
		Controller2D(CharacterSprite* s) {
			sprite = s;
			animController = new AnimationTickController();
			animController->addAnimatedCharacter(sprite);
		}

		inline void setPosition(glm::vec2 pos) {
			position = pos;
			sprite->position(pos.x, pos.y);
		}

		inline void update(float dt) {
			position += velocity * dt;
			velocity = { 0, 0 };
			setPosition(position);
		}
		
		inline void draw() {
			animController->tick();
			sprite->draw();
		}

		inline AnimationTickController* getAnimController() {
			return animController;
		}

		inline void registerHandlers() {

		}

		inline CharacterSprite* getCharacterSprite() {
			return sprite;
		}

		glm::vec2 position;
		glm::vec2 velocity;

	protected:
		CharacterSprite* sprite;
		AnimationTickController* animController;
	};
}