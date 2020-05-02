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
			tmaps.clear();
			tmap2s.clear();
			sprts.clear();
		}

		inline void setPosition(glm::vec2 pos) {
			position = pos;
			sprite->position(pos.x, pos.y);
		}

		inline void update(float dt) {
			glm::vec2 newPos = position + velocity * dt;

			doPhysics(newPos);

			setPosition(position);
		}

		inline void doPhysics(glm::vec2 p) {
			bool collide = false;

			for (auto t : tmaps) {
				if (collide)
					break;
				collide |= t->checkPhysics(sprite->getAABB());
			}

			for (auto t : tmap2s) {
				if (collide)
					break;
				collide |= t->checkPhysics(sprite->getAABB());
			}

			for (auto t : sprts) {
				if (collide)
					break;
				collide |= t->checkPhysics(sprite->getAABB());
			}

			if (!collide) {
				position = p;
			}

			velocity = { 0, 0 };
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

		inline void addPhysicsTileMap(Tilemap* tmap) {
			tmaps.push_back(tmap);
		}
		inline void addPhysicsTileMap(TilemapAnim* tmap) {
			tmap2s.push_back(tmap);
		}
		inline void addPhysicsSprite(Sprite2* sprt) {
			sprts.push_back(sprt);
		}

		inline void clearPhysics() {
			tmaps.clear();
			tmap2s.clear();
			sprts.clear();
		}

		glm::vec2 position;
		glm::vec2 velocity;

	protected:
		CharacterSprite* sprite;
		AnimationTickController* animController;
		std::vector<Tilemap*> tmaps;
		std::vector<TilemapAnim*> tmap2s;
		std::vector<Sprite2*> sprts;

	};
}