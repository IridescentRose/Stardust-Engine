#include <GFX/2D/Sprite.h>

namespace Stardust::GFX::Render2D{
	Sprite::Sprite()
	{
		tex = -1;
		offset = { 0, 0 };
		scaleFactor = { 1.0, 1.0 };
	}

	Sprite::~Sprite()
	{
		model.deleteData();
	}

	Sprite::Sprite(unsigned int t, glm::vec2 size)
	{
		tex = t;

		mesh.position = {
		   -size.x,-size.y, 0,
			size.x,-size.y, 0,
			size.x, size.y, 0,
		   -size.x, size.y, 0,
		};

		mesh.color = {
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
		};

		Texture* tD = g_TextureManager->getTex(t);
		float hPercent = (float)tD->height / (float)tD->pHeight;
		float wPercent = (float)tD->width / (float)tD->pWidth;

		mesh.uv = {
			0, 0,
			hPercent, 0,
			hPercent, wPercent,
			0, wPercent
		};

		mesh.indices = {
			0, 1, 2, 2, 3, 0
		};

		model.addData(&mesh);
	}

	Sprite::Sprite(unsigned int t, glm::vec2 pos, glm::vec2 extent)
	{
		tex = t;

		Texture* tD = g_TextureManager->getTex(t);

		mesh.color = {
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
		};

		float hstart = (float)pos.y / (float)tD->pHeight;
		float wstart = (float)pos.x / (float)tD->pWidth;
		float hPercent = (float)(pos.y + extent.y) / (float)tD->pHeight;
		float wPercent = (float)(pos.x + extent.x) / (float)tD->pWidth;

		mesh.uv = {
			wstart, hstart,
			wPercent, hstart,
			wPercent, hPercent,
			wstart, hPercent,
		};

		mesh.position = {
			0, 0, 0,
			pos.x, 0, 0,
			pos.x, extent.y, 0,
			0, extent.y, 0,
		};

		mesh.indices = {
			0, 1, 2, 2, 3, 0
		};

		model.addData(&mesh);
	}

	void Sprite::setLayer(int i)
	{
		mesh.position.clear();
		mesh.position[0*3 + 2] = i;
		mesh.position[1*3 + 2] = i;
		mesh.position[2*3 + 2] = i;
		mesh.position[3*3 + 2] = i;
		//model.addData(&mesh);
	}

	void Sprite::setPosition(float x, float y)
	{
		offset = { x, y };
	}

	void Sprite::setScale(float x, float y)
	{
		scaleFactor = { x, y };
	}

	void Sprite::setColor(float r, float g, float b, float a)
	{
		mesh.color.clear();
		mesh.color = {
			r, g, b, a,
			r, g, b, a,
			r, g, b, a,
			r, g, b, a,
		};

		model.addData(&mesh);
	}

	void Sprite::setTexture(unsigned int t)
	{
		tex = t;
	}

	void Sprite::draw()
	{

		//Matrix translation

		g_TextureManager->bindTex(tex);
		model.draw();
	}
	

}

