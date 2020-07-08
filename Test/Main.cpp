#include <Platform/Platform.h>
#include <Utilities/Input.h>
#include <Utilities/Logger.h>
#include <Graphics/RendererCore.h>
#include <Network/NetworkDriver.h>
#include <Graphics/3D/Mesh.h>
#include <Graphics/TextureUtil.h>

PSP_MODULE_INFO("Stardust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

using namespace Stardust;
using namespace Stardust::Graphics;

int main() {
	Platform::initPlatform();

	Render3D::Camera* cam = new Render3D::Camera({ 0, 0, 0 }, { 0, 0, 0 }, 90.0f, 480.0f / 272.0f, 0.1f, 1000.0f);
	g_RenderCore.SetCamera(cam);
	g_RenderCore.Set3DMode();


	Render3D::Mesh* mesh = new Render3D::Mesh();
	mesh->addVertex({0.0f, 0.0f, 0xFFFFFFFF, 0.f, 0.f, -5.f });
	mesh->addVertex({1.0f, 0.0f, 0xFFFFFFFF, 1.f, 0.f, -5.f });
	mesh->addVertex({ 1.0f, 1.0f, 0xFFFFFFFF, 1.f, 1.f, -5.f });
	mesh->addVertex({ 1.0f, 1.0f, 0xFFFFFFFF, 1.f, 1.f, -5.f });
	mesh->addVertex({ 0.0f, 1.0f, 0xFFFFFFFF, 0.f, 1.f, -5.f });
	mesh->addVertex({ 0.0f, 0.0f, 0xFFFFFFFF, 0.f, 0.f, -5.f });

	Texture* tex = TextureUtil::LoadTex("example.jpg");

	while (!Utilities::KeyPressed(PSP_CTRL_START)) {
		g_RenderCore.BeginCommands();
		g_RenderCore.Clear();

		cam->rot.y += 1.0f;
		cam->update();

		tex->bindTexture();
		mesh->drawMesh();

		g_RenderCore.EndCommands();
		Platform::platformUpdate();
	}

	Platform::exitPlatform();
	return 0;
}