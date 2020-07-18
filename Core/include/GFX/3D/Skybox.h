#pragma once
#include <GFX/GFXWrapper.h>

namespace Stardust::GFX::Render3D{
	class Skybox{
	public:

		Skybox(std::array<std::string, 6> fileNames, float zDist);
		~Skybox();

		void draw();

	private:
		Mesh face[6];
		Model faces[6];

		unsigned int texID[6];
	};
}