#pragma once
#include <Core/Core.h>
#include <GFX/GFXWrapper.h>
#include <GFX/RenderCore.h>

namespace Stardust::Component {

	class ModelComponent : Core::Component {
	public:
		ModelComponent() = default;

		inline void setModel(GFX::Model* mod) {
			model = mod;
		}

		inline void Init() {
			model = nullptr;
		}
		inline void Update() {};
		
		void Draw();

	private:
		GFX::Model* model;
	};
}