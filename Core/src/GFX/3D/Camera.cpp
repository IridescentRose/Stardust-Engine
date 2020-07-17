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

		proj = glm::perspective(fovy, aspect, zN, zF);
	}

	void Camera::update()
	{
		view = glm::mat4(1.0f);

		glm::rotate(view, rot.x / 180.0f * 3.14159f, { 1, 0, 0 });
		glm::rotate(view, rot.y / 180.0f * 3.14159f, { 0, 1, 0 });
		glm::rotate(view, rot.z / 180.0f * 3.14159f, { 0, 0, 1 });
	}
}