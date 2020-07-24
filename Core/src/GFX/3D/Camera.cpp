#include <GFX/3D/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Stardust::GFX::Render3D {
	Camera::Camera(glm::vec3 position, glm::vec3 rotation, float fovy, float aspect, float zN, float zF)
	{
		pos = position;
		rot = rotation;


		setProjection(fovy, aspect, zN, zF);
		update();
	}

	void Camera::setProjection(float fovy, float aspect, float zN, float zF)
	{
		fov = fovy;
		aspectRatio = aspect;
		zNear = zN;
		zFar = zF;

#if CURRENT_PLATFORM == PLATFORM_PSP
		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumPerspective(fovy, aspect, zN, zF);
		sceGumStoreMatrix(&proj);
#else
		proj = glm::perspective(glm::radians(fovy), aspect, zN, zF);
#endif
	}

	void Camera::update()
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();

		sceGumRotateX(rot.x / 180.0f * 3.14159f);
		sceGumRotateY(rot.y / 180.0f * 3.14159f);
		sceGumRotateZ(rot.z / 180.0f * 3.14159f);
		ScePspFVector3 v = { pos.x, pos.y, pos.z };
		sceGumTranslate(&v);
		sceGumStoreMatrix(&view);

		sceGumMatrixMode(GU_MODEL);
#else
		glm::mat4 matrix(1.f);

		matrix = glm::rotate(matrix, glm::radians(rot.x), { 1, 0, 0 });
		matrix = glm::rotate(matrix, glm::radians(rot.y), { 0, 1, 0 });
		matrix = glm::rotate(matrix, glm::radians(rot.z), { 0, 0, 1 });

		matrix = glm::translate(matrix, -pos);

		view =  matrix;
#endif
	}
}