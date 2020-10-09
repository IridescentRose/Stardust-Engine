#include <Components/LODModelComponent.h>

namespace Stardust::Component {
	inline void LODModelComponent::Update()
	{
		if (model != nullptr) {
			if (GFX::g_RenderCore->renderCam != nullptr) {
				model->update(GFX::g_RenderCore->renderCam->pos);
			}
			else {
				model->update({ 0, 0, 0 });
			}
		}
	}
	void LODModelComponent::Draw()
	{
		if (model != nullptr) {
			GFX::pushMatrix();
			if (parent != nullptr) {
				GFX::scaleModelMatrix(parent->getTransform().scale);
				GFX::rotateModelMatrix(parent->getTransform().rot);
				GFX::translateModelMatrix(parent->getTransform().pos);
			}

			model->draw();

			GFX::popMatrix();
		}
	}
}