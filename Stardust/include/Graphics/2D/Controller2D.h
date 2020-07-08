#pragma once
#include "CharacterSprite.h"
#include <Core/Controller.h>
#include "AnimTickController.h"

namespace Stardust::Graphics::Render2D {
	class Controller2D : public Core::ControllerBase {
	public:
		Controller2D(CharacterSprite* s, bool camFollow = true) {
			sprite = s;
			animController = new AnimationTickController();
			animController->addAnimatedCharacter(sprite);
			tmaps.clear();
			tmap2s.clear();
			sprts.clear();
			follow = camFollow;
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
				Math::AABB2D aabb = sprite->getAABB();
				aabb.offset = p;
				collide |= t->checkPhysics(aabb);
			}

			for (auto t : tmap2s) {
				if (t != NULL) {

					if (collide)
						break;
					Math::AABB2D aabb = sprite->getAABB();
					aabb.offset = p;
					collide |= t->checkPhysics(aabb);
				}
			}

			for (auto t : sprts) {
				if (collide)
					break;
				Math::AABB2D aabb = sprite->getAABB();
				aabb.offset = p;
				collide |= t->checkPhysics(aabb);
			}

			if (!collide) {
				position = p;
			}

			velocity = { 0, 0 };
		}
		
		inline void draw() {
			animController->tick();
			if (follow) {
				sceGumMatrixMode(GU_VIEW);
				sceGumLoadIdentity();
				ScePspFVector3 v = { 240- position.x, 136 - position.y, 0.0f };
				sceGumTranslate(&v);
				sceGumMatrixMode(GU_MODEL);
			}

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
		inline int addPhysicsTileMap(TilemapAnim* tmap) {
			int idx = tmap2s.size();
			tmap2s.push_back(tmap);
			return idx;
		}
		inline void removePhysicsTileMapAnim(int idx) {
			tmap2s.erase(tmap2s.begin() + idx);
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
		bool follow;

	};
}