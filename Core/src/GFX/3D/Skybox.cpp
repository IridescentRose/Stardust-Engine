#include <GFX/3D/Skybox.h>

namespace Stardust::GFX::Render3D{
	Skybox::Skybox(std::array<std::string, 6> fileNames, float zDist)
	{
		for(int i = 0; i < 6; i++){
			texID[i] = g_TextureManager->loadTex(fileNames[i], GFX_FILTER_LINEAR, GFX_FILTER_LINEAR, false);
		}

		for (int i = 0; i < 6; i++) {
			face[i].color = {
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
			};
			face[i].indices = {
				3, 2, 1, 1, 0, 3
			};

			face[i].uv = {
				0, 0, 
				1, 0, 
				1, 1,
				0, 1
			};
		}

		face[0].position = {
			zDist, zDist, -zDist,
			zDist, zDist, zDist,
			zDist, -zDist, zDist,
			zDist, -zDist, -zDist,
		};


		face[1].position = {
			-zDist, zDist, zDist,
			-zDist, zDist, -zDist,
			-zDist, -zDist, -zDist,
			-zDist, -zDist, zDist,
		};


		face[2].position = {
			zDist, zDist, zDist,
			-zDist, zDist, zDist,
			-zDist, -zDist, zDist,
			zDist, -zDist, zDist,
		};

		face[3].position = {
			-zDist, zDist,	-zDist,
			zDist, zDist,	-zDist,
			zDist, -zDist,	-zDist,
			-zDist, -zDist, -zDist,
		};

		face[4].position = {
			zDist, -zDist, -zDist,
			zDist, -zDist, zDist,
			-zDist, -zDist, zDist,
			-zDist, -zDist, -zDist,
		};

		face[5].position = {
		   -zDist,	zDist, -zDist,
		   -zDist,	zDist,	zDist,
			zDist,	zDist,	zDist,
			zDist,	zDist, -zDist,
		};

		for(int i = 0; i < 6; i++){
			faces[i].addData(face[i]);
		}

	}

	Skybox::~Skybox()
	{
		for (int i = 0; i < 6; i++) {
			g_TextureManager->deleteTex(texID[i]);
		}
	}

	void Skybox::draw()
	{
		for(int i = 0; i < 6; i++){
			g_TextureManager->bindTex(texID[i]);
			faces[i].draw();
		}
	}
}