#pragma once
#include <Core/Core.h>
#include <GFX/3D/LODModel.h>
#include <GFX/RenderCore.h>

namespace Stardust::Component {

	class LODModelComponent : Core::Component {
	public:
		LODModelComponent() = default;

		inline void setModel(GFX::Render3D::LODModel* mod) {
			model = mod;
			model->position = parent->getTransform().pos;
		}

		inline void Init() {
			model = nullptr;
		}
		inline void Update();

		void Draw();

	private:
		GFX::Render3D::LODModel* model;
	};
}