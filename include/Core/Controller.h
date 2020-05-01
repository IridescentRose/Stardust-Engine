#pragma once
#include <Math/Vector.h>

namespace Stardust::Core {
	class ControllerBase {
	public:
		ControllerBase() {

		}

		virtual void walkForward(bool, float) = 0;
		virtual void walkBackward(bool, float) = 0;
		virtual void walkLeft(bool, float) = 0;
		virtual void walkRight(bool, float) = 0;

		virtual void update(float dt) = 0;
		virtual void draw() = 0;

		virtual void setPosition() = 0;

	private:
		
	};

	class Controller2D : public ControllerBase {
	public:
		Controller2D() {

		}

	protected:
		glm::vec2 position;
		glm::vec2 velocity;
	};
}