#pragma once
#include <cstdint>

namespace Stardust::GFX::Render2D {
	struct PointLight {
		int x, y;
		uint8_t r, g, b, a;
		float range;
		float intensity;
	};

	struct AmbientLight {
		uint8_t r, g, b, a;
		float intensity;
	};
}