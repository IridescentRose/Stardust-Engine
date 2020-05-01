#pragma once
#include <Core/Controller.h>

namespace Stardust::Graphics::Render2D{
	class TopDownController : public Core::Controller2D {
	public:

		void walkForward(bool, float) = 0;
		void walkBackward(bool, float) = 0;
		void walkLeft(bool, float) = 0;
		void walkRight(bool, float) = 0;

	private:

	};
}