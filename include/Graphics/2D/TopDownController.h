#pragma once
#include "Controller2D.h"

namespace Stardust::Graphics::Render2D{
	class TopDownController : public Controller2D {
	public:
		TopDownController(CharacterSprite* s, float speed);
		void registerHandlers();

		float speed;
	};
}