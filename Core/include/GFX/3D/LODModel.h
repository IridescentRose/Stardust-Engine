#pragma once
#include <GFX/GFXWrapper.h>
#include <Platform/Platform.h>
#include <glm/glm.hpp>
#include <vector>

namespace Stardust::GFX::Render3D {
	class LODModel {
	public:
		LODModel();

		void draw();
		void update(glm::vec3 camPos);

		glm::vec3 position;

		void addLODLevel(Model* model, float distance);

	private:
		std::vector<float> distanceMap;
		std::vector<Model*> modelMap;
		int lodLevel;
	};
}