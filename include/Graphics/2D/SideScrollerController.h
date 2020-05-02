#pragma once
#include "Controller2D.h"

namespace Stardust::Graphics::Render2D {
	class SideScrollerController : public Controller2D {
	public:
		SideScrollerController(CharacterSprite* s, float speed, float gravitySpeed, float jump, bool follow = true);
		void registerHandlers();

		void setGravity(float gravity);
		void setSpeed(float speed);

		void setJump(float jump);

		void update(float dt);

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
				if (collide)
					break;
				Math::AABB2D aabb = sprite->getAABB();
				aabb.offset = p;
				collide |= t->checkPhysics(aabb);
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

				velocity = { 0, 0 };
			}
			else {

				velocity = { 0, 0 };
			}
		}

		float speed;
		float jump;
		float gravity;

		float gravityVelocity;
		bool isOnGround;
	};
}