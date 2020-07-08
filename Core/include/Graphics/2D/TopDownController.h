#pragma once
#include "Controller2D.h"

namespace Stardust::Graphics::Render2D{
	class TopDownController : public Controller2D {
	public:
		TopDownController(CharacterSprite* s, float speed, bool follow = true);
		void registerHandlers();

		void update(float dt);

		float speed;
	};
}