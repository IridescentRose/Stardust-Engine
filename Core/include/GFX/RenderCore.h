#pragma once
#include <Platform/Platform.h>
#include <Platform/Graphics.h>

namespace Stardust::GFX {
	class RenderCore {
	public:
		RenderCore();
		~RenderCore();

		void init();
		void cleanup();

		void clear();
		void setClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void setClearColor(float r, float g, float b, float a);

		void setDefault2DMode();
		void set2DMode(float left, float right, float bottom, float top, float znear, float zfar);

		void beginFrame();
		void endFrame();

	private:
		uint8_t r, g, b, a;
	};

	extern RenderCore* g_RenderCore;

}