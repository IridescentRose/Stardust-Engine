#include <GFX/2D/SpriteAnimated.h>

namespace Stardust::GFX::Render2D {
	SpriteAnimated::SpriteAnimated()
	{
	}
	SpriteAnimated::SpriteAnimated(unsigned int tex, glm::vec2 size, char numberPhases, bool hz) : Sprite(tex, size)
	{
		phaseNumber = 0;
		phaseTotal = numberPhases;


		horizontal = hz;

		mesh.uv.clear();
		if (!hz) {
			mesh.uv = {
				0, 0,
				1, 0,
				1, 1.0f / (float)numberPhases,
				0, 1.0f / (float)numberPhases,
			};
		}
		else {
			mesh.uv = {
				0, 0,
				1.0f / (float)numberPhases, 0,
				1.0f / (float)numberPhases, 1,
				0, 1,
			};
		}

		model.addData(mesh);
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
		mesh.uv.clear();

		if (!horizontal) {
			mesh.uv = {
				0, posBegin,
				1, posBegin,
				1, posEnd,
				0, posEnd
			};
		}
		else {
			mesh.uv = {
				posBegin, 0,
				posEnd, 0,
				posEnd, 1,
				posBegin, 1,
			};
		}
		model.addData(mesh);
	}
}