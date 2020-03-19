#pragma once

namespace Stardust::Graphics {
	union Color {
		unsigned int hex;
		struct components {
			unsigned char a, r, g, b;
		} components;
	};




}