#pragma once
#include "SpriteAdvanced.h"

namespace Stardust::Graphics::Render2D {
	class SpriteAnimated : public Sprite2 {
	public:
		SpriteAnimated();
		SpriteAnimated(Texture* tex, glm::vec2 size, char numberPhases, bool horizontal = false);

		void tickPhase();
		void setTick(char i);

	private:
		char phaseNumber;
		char phaseTotal;
		bool horizontal;
		typedef Sprite2 super;
	};
}