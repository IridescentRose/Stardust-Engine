#include <Graphics/2D/SpriteAnimated.h>

namespace Stardust::Graphics::Render2D {
	SpriteAnimated::SpriteAnimated()
	{
	}
	SpriteAnimated::SpriteAnimated(Texture* tex, glm::vec2 size, char numberPhases, bool hz) : Sprite2(tex, size)
	{
		phaseNumber = 0;
		phaseTotal = numberPhases;

		vertices[0].u = 0;
		vertices[0].v = 0;

		horizontal = hz;

		if (!hz) {
			vertices[1].u = 1.0f;
			vertices[1].v = 1.0f / (float)numberPhases;
		}
		else {
			vertices[1].u = 1.0f / (float)numberPhases;
			vertices[1].v = 1.0f;
		}
	}
	void SpriteAnimated::tickPhase()
	{
		phaseNumber++;
		setTick(phaseNumber);
	}
	void SpriteAnimated::setTick(char i)
	{
		phaseNumber = i;

		if (phaseNumber >= phaseTotal) {
			phaseNumber = 0;
		}

		float posBegin = (float)phaseNumber / (float)phaseTotal;
		float posEnd = (float)(phaseNumber + 1) / (float)phaseTotal;

		if (!horizontal) {
			vertices[0].u = 0.0f;
			vertices[0].v = posBegin;

			vertices[1].u = 1.0f;
			vertices[1].v = posEnd;
		}
		else {
			vertices[0].u = posBegin;
			vertices[0].v = 0.0f;

			vertices[1].u = posEnd;
			vertices[1].v = 1.0f;
		}
	}
}