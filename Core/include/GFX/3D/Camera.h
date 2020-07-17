#pragma once
#include <glm/glm.hpp>

namespace Stardust::GFX::Render3D {
	class Camera {
	public:
		Camera(glm::vec3 position, glm::vec3 rotation, float fovy, float aspect, float zN, float zF);

		void setProjection(float fovy, float aspect, float zN, float zF);
		void update();

		inline glm::mat4 getProjection() {
			return proj;
		}
		inline glm::mat4 getView() {
			return view;
		}

		glm::vec3 pos;
		glm::vec3 rot;
	private:

		float fov, aspectRatio, zNear, zFar;

		glm::mat4 proj, view;
	};
}