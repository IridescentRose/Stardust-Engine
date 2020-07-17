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

		scaleFactor = { 1.0, 1.0 };
		mesh.position = {
		   -size.x / 2.0f,-size.y / 2.0f, 0, //0
			size.x / 2.0f,-size.y / 2.0f, 0, //1
			size.x/2.0f, size.y/2.0f, 0, //2
		   -size.x / 2.0f, size.y / 2.0f, 0, //3
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
			3, 2, 1, 1, 0, 3
		};

		model.addData(mesh);
	}

	Sprite::Sprite(unsigned int t, glm::vec2 pos, glm::vec2 extent)
	{
		tex = t;

		scaleFactor = { 1.0, 1.0 };
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

		model.addData(mesh);
	}

	void Sprite::setLayer(int i)
	{
		mesh.position.clear();
		mesh.position[0*3 + 2] = i;
		mesh.position[1*3 + 2] = i;
		mesh.position[2*3 + 2] = i;
		mesh.position[3*3 + 2] = i;
		model.addData(mesh);
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

		model.addData(mesh);
	}

	void Sprite::setTexture(unsigned int t)
	{
		tex = t;
	}

	void Sprite::draw()
	{

		//Matrix translation
		GFX::pushMatrix();
		GFX::clearModelMatrix();
		GFX::scaleModelMatrix(glm::vec3(scaleFactor.x, scaleFactor.y, 1.0f));
		GFX::translateModelMatrix(glm::vec3(offset.x, offset.y, 1.0f));

		g_TextureManager->bindTex(tex);
		model.draw();

		GFX::popMatrix();
	}
	
	void Sprite::setAmbientLight(AmbientLight light)
	{
		ambient = light;
	}

	void Sprite::addPointLight(PointLight light)
	{
		pointLights.push_back(light);
	}
	void Sprite::calculateLighting()
	{
		uint8_t r = (float)ambient.r * ambient.intensity;
		uint8_t g = (float)ambient.g * ambient.intensity;
		uint8_t b = (float)ambient.b * ambient.intensity;

		for (auto p : pointLights) {
			float distance = sqrtf((p.x - offset.x * 2.0f) * (p.x - offset.x * 2.0f) + (p.y - offset.y * 2.0f) * (p.y - offset.y * 2.0f));
			float corrRange = (p.range * 10 - distance) / (p.range * 10);

			float intensityMax = 1.0f - ambient.intensity;

			float intensity = 0.0f;
			if (p.intensity > intensityMax) {
				intensity = intensityMax;
			}
			else {
				intensity = p.intensity;
			}

			if (corrRange > 1.0f) {
				corrRange = 1.0f;
			}

			if (corrRange > 0.0f) {
				r += ((float)p.r) * intensity * corrRange;
				g += ((float)p.g) * intensity * corrRange;
				b += ((float)p.b) * intensity * corrRange;
			}

			if (r > 255) {
				r = 255;
			}
			if (g > 255) {
				g = 255;
			}
			if (b > 255) {
				b = 255;
			}

			if (r < 0) {
				r = 0;
			}
			if (g < 0) {
				g = 0;
			}
			if (b < 0) {
				b = 0;
			}
		}

		setColor( (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
	}
	void Sprite::clearPointLights()
	{
		pointLights.clear();
	}
}

