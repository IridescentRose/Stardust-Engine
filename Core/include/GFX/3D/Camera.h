#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <GFX/GFXWrapper.h>
#include <Platform/Platform.h>

namespace Stardust::GFX::Render3D {
	class Camera {
	public:
		Camera(glm::vec3 position, glm::vec3 rotation, float fovy, float aspect, float zN, float zF);

		void setProjection(float fovy, float aspect, float zN, float zF);
		void update();

#if CURRENT_PLATFORM == PLATFORM_PSP
		inline ScePspFMatrix4 getProjection() {
			return proj;
		}
		inline ScePspFMatrix4 getView() {
			return view;
		}
#else
		inline glm::mat4 getProjection() {
			return proj;
		}
		inline glm::mat4 getView() {
			return view;
		}
#endif
		

		glm::vec3 pos;
		glm::vec3 rot;
	private:

		float fov, aspectRatio, zNear, zFar;

#if CURRENT_PLATFORM == PLATFORM_PSP
		ScePspFMatrix4 proj, view;
#else
		glm::mat4 proj, view;
#endif
	};
}