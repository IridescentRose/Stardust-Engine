#pragma once
#include "Sprite.h"

namespace Stardust::GFX::Render2D {
	class SpriteAnimated : public Sprite {
	public:
		SpriteAnimated();
		SpriteAnimated(unsigned int tex, glm::vec2 size, char numberPhases, bool horizontal = false);

		void tickPhase();
		void setTick(char i);

	private:
		char phaseNumber;
		char phaseTotal;
		bool horizontal;
	};
}