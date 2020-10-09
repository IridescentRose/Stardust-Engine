#include <GFX/3D/LODModel.h>

namespace Stardust::GFX::Render3D {
	LODModel::LODModel()
	{
		modelMap.clear();
		position = { 0, 0, 0 };
		lodLevel = 0;
	}

	void LODModel::draw()
	{
		if (modelMap.size() > lodLevel) {
			if (modelMap[lodLevel] != nullptr) {
				modelMap[lodLevel]->bind();
				modelMap[lodLevel]->draw();
			}
		}
	}

	void LODModel::update(glm::vec3 camPos)
	{
		float currDistance = sqrtf((camPos.x - position.x) * (camPos.x - position.x) + (camPos.y - position.y) * (camPos.y - position.y) + (camPos.z - position.z) * (camPos.z - position.z));
		
		int guess = 0;
		for (int i = 0; i < distanceMap.size(); i++) {
			if (currDistance > distanceMap[i]) {
				guess = i;
			}
			else {
				//If it's less than the next distance, we should assume we're done.
				break;
			}
		}

	}

	//Add LODs in order of closest to furthest
	//LOD0 = highest detail
	//LODn = lowest detail
	void LODModel::addLODLevel(Model* model, float distance)
	{
		modelMap.push_back(model);
		distanceMap.push_back(distance);
	}
}