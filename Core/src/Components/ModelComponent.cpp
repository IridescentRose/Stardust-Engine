#include <Components/ModelComponent.h>

namespace Stardust::Component {
	void ModelComponent::Draw()
	{
		if (model != nullptr) {
			GFX::pushMatrix();
			if (parent != nullptr) {
				GFX::scaleModelMatrix(parent->getTransform().scale);
				GFX::rotateModelMatrix(parent->getTransform().rot);
				GFX::translateModelMatrix(parent->getTransform().pos);
			}

			model->bind();
			model->draw();

			GFX::popMatrix();
		}
	}
}