#pragma once
#include <Platform/Platform.h>
#include <Platform/Graphics.h>
#include <GFX/3D/Camera.h>

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

		void bindCamera(Render3D::Camera* cam);
		void set3DMode();

	private:
		uint8_t r, g, b, a;
		Render3D::Camera* renderCam;
	};

	extern RenderCore* g_RenderCore;

}