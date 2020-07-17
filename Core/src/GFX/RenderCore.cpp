#include <GFX/RenderCore.h>
#include <GFX/GFXWrapper.h>

namespace Stardust::GFX {
	RenderCore::RenderCore()
	{
		r = g = b = a = 0;
	}
	RenderCore::~RenderCore()
	{
		r = g = b = a = 0;
	}
	void RenderCore::init()
	{
		renderCam = NULL;
		Platform::detail::Graphics::initGraphicsContext();
		setDefault2DMode();
	}
	void RenderCore::cleanup()
	{
		Platform::detail::Graphics::cleanupGraphicsContext();
	}

	void RenderCore::clear()
	{
		gfxClear(GFX_COLOR_BUFFER | GFX_DEPTH_BUFFER | GFX_STENCIL_BUFFER);
	}
	void RenderCore::setClearColor(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;

		gfxClearColor(((float)r)/255.0f, ((float)g)/255.0f, ((float)b)/255.0f, ((float)a)/255.0f); 
	}

	void RenderCore::setClearColor(float _r, float _g, float _b, float _a)
	{
		r = _r * 255.0f;
		g = _g * 255.0f;
		b = _b * 255.0f;
		a = _a * 255.0f;
		gfxClearColor(_r, _g, _b, _a);
	}

	void RenderCore::set2DMode(float left, float right, float bottom, float top, float znear, float zfar)
	{
		gfxSetOrtho(left, right, bottom, top, znear, zfar);
	}

	void RenderCore::setDefault2DMode(){
		set2DMode(0, 480, 272, 0, -30, 30);
	}

	void RenderCore::beginFrame()
	{
		Platform::detail::Graphics::beginFrame();
	}
	void RenderCore::endFrame()
	{
		Platform::detail::Graphics::endFrame();
	}

	void RenderCore::bindCamera(Render3D::Camera* cam)
	{
		renderCam = cam;
	}

	void RenderCore::set3DMode()
	{
		//Bind our camera's rendering settings.
		if(renderCam != NULL){
			//Bind projection and view data.
			gfxSetProjView(renderCam->getProjection(), renderCam->getView());
		}
	}

	RenderCore* g_RenderCore = nullptr;
}