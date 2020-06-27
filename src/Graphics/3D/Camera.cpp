#include <Graphics/3D/Camera.h>
#include <pspgu.h>
#include <pspgum.h>

namespace Stardust::Graphics::Render3D {
	Camera::Camera(glm::vec3 position, glm::vec3 rotation, float fovy, float aspect, float zN, float zF)
	{
		pos = position;
		rot = rotation;

		setProjection(fovy, aspect, zN, zF);
		update();
	}

	void Camera::setProjection(float fovy, float aspect, float zN, float zF)
	{
		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();

		sceGumPerspective(fovy, aspect, zN, zF);

		fov = fovy;
		aspectRatio = aspect;
		zNear = zN;
		zFar = zF;

		sceGumStoreMatrix(&proj);
	}

	void Camera::update()
	{
		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumLoadMatrix(&proj);

		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();

		sceGumRotateX(rot.x / 180.0f * 3.14159f);
		sceGumRotateY(rot.y / 180.0f * 3.14159f);
		sceGumRotateZ(rot.z / 180.0f * 3.14159f);

		ScePspFVector3 v = { pos.x, pos.y, pos.z };
		sceGumTranslate(&v);

		sceGumStoreMatrix(&view);
		sceGumLoadMatrix(&view);

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
	}
}