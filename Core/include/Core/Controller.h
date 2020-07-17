#pragma once
#include <Math/Vector.h>

namespace Stardust::Core {
	class ControllerBase {
	public:
		ControllerBase() {

		}
		virtual void registerHandlers() = 0;

		virtual void update(float dt) = 0;
		virtual void draw() = 0;

	private:
		
	};
}