#include <GFX/2D/TilemapAnim.h>

namespace Stardust::GFX::Render2D {
	TilemapAnim::TilemapAnim(TextureAtlas* atlas, unsigned int texture)
	{
		m_Atlas = atlas;
		texRef = texture;

		m_Tiles.clear();
		position = { 0, 0 };
	}
	TilemapAnim::~TilemapAnim()
	{
		clearTiles();
	}
	int TilemapAnim::addTile(TileAnim* t)
	{
		int idx = m_Tiles.size();
		m_Tiles.push_back(t);
		return idx;
	}
	void TilemapAnim::updateTile(int tileNum, TileAnim* t)
	{
		delete m_Tiles[tileNum];
		m_Tiles[tileNum] = t;
	}

	void TilemapAnim::setAmbientLight(AmbientLight light)
	{
		ambient = light;
	}

	void TilemapAnim::addPointLight(PointLight light)
	{
		pointLights.push_back(light);
	}

	void TilemapAnim::calculateLighting()
	{
		for (auto t : m_Tiles) {
			uint8_t r = (float)ambient.r * ambient.intensity;
			uint8_t g = (float)ambient.g * ambient.intensity;
			uint8_t b = (float)ambient.b * ambient.intensity;

			for (auto p : pointLights) {
				float distance = sqrtf((p.x - t->offset.x) * (p.x - t->offset.x) + (p.y - t->offset.y) * (p.y - t->offset.y));
				float corrRange = (p.range - distance) / p.range;

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

			t->r = r;
			t->g = g;
			t->b = b;
		}

		buildMap();
	}

	void TilemapAnim::tickPhase()
	{
		for (int i = 0; i < m_Tiles.size(); i++) {
			auto t = m_Tiles[i];

			if (!t->isAnim)
				continue;

			t->tickNumber++;
			if (t->tickNumber > t->animLength) {
				t->tickNumber = 0;
			}

			std::array<float, 8> coords = m_Atlas->getTexture(t->indexStart + t->tickNumber);


			mesh.uv[i*8 + 0] = (coords[(0 + t->rotation) % 8]);
			mesh.uv[i*8 + 1] = (coords[(1 + t->rotation) % 8]);
			mesh.uv[i*8 + 2] = (coords[(4 + t->rotation) % 8]);
			mesh.uv[i*8 + 3] = (coords[(1 + t->rotation) % 8]);
			mesh.uv[i*8 + 4] = (coords[(4 + t->rotation) % 8]);
			mesh.uv[i*8 + 5] = (coords[(5 + t->rotation) % 8]);
			mesh.uv[i*8 + 6] = (coords[(0 + t->rotation) % 8]);
			mesh.uv[i*8 + 7] = (coords[(5 + t->rotation) % 8]);

		}
		model.addData(mesh);
	}


	bool TilemapAnim::checkPhysics(const Math::AABB2D& aabb)
	{
		for (auto t : m_Tiles) {

			if (t->physics) {
				Math::AABB2D b = { {t->offset.x, t->offset.y + 272}, t->extent };
				b.offset += position;
				bool res = Math::AABBIntersect2D(aabb, b);

				if (res)
					return res;
			}
		}
		return false;
	}

	void TilemapAnim::clearTiles()
	{
		for (auto t : m_Tiles) {
			delete t;
		}
		m_Tiles.clear();
	}
	void TilemapAnim::setPosition(glm::vec2 pos)
	{
		position = pos;
	}
	void TilemapAnim::buildMap()
	{
		mesh.position.clear();
		mesh.uv.clear();
		mesh.color.clear();
		mesh.indices.clear();

		unsigned int index = 0;

		for (auto t : m_Tiles) {
			std::array<float, 8> coords;
			if (t->isAnim) {
				coords = m_Atlas->getTexture(t->indexStart + t->tickNumber);
			}
			else {
				coords = m_Atlas->getTexture(t->texIndex);
			}

			mesh.uv.push_back(coords[(0 + t->rotation) % 8]);
			mesh.uv.push_back(coords[(1 + t->rotation) % 8]);
			mesh.uv.push_back(coords[(4 + t->rotation) % 8]);
			mesh.uv.push_back(coords[(1 + t->rotation) % 8]);
			mesh.uv.push_back(coords[(4 + t->rotation) % 8]);
			mesh.uv.push_back(coords[(5 + t->rotation) % 8]);
			mesh.uv.push_back(coords[(0 + t->rotation) % 8]);
			mesh.uv.push_back(coords[(5 + t->rotation) % 8]);

			mesh.color.push_back((float)t->r / 255.0f);
			mesh.color.push_back((float)t->g / 255.0f);
			mesh.color.push_back((float)t->b / 255.0f);
			mesh.color.push_back((float)t->a / 255.0f);
			mesh.color.push_back((float)t->r / 255.0f);
			mesh.color.push_back((float)t->g / 255.0f);
			mesh.color.push_back((float)t->b / 255.0f);
			mesh.color.push_back((float)t->a / 255.0f);
			mesh.color.push_back((float)t->r / 255.0f);
			mesh.color.push_back((float)t->g / 255.0f);
			mesh.color.push_back((float)t->b / 255.0f);
			mesh.color.push_back((float)t->a / 255.0f);
			mesh.color.push_back((float)t->r / 255.0f);
			mesh.color.push_back((float)t->g / 255.0f);
			mesh.color.push_back((float)t->b / 255.0f);
			mesh.color.push_back((float)t->a / 255.0f);

			mesh.indices.push_back(index + 3);
			mesh.indices.push_back(index + 2);
			mesh.indices.push_back(index + 1);
			mesh.indices.push_back(index + 1);
			mesh.indices.push_back(index + 0);
			mesh.indices.push_back(index + 3);


			if (t->rotation == 0 || t->rotation == 4) {

				mesh.position.push_back(t->offset.x);
				mesh.position.push_back(t->offset.y);
				mesh.position.push_back(t->layer);

				mesh.position.push_back(t->offset.x + t->extent.x);
				mesh.position.push_back(t->offset.y);
				mesh.position.push_back(t->layer);

				mesh.position.push_back(t->offset.x + t->extent.x);
				mesh.position.push_back(t->offset.y + t->extent.y);
				mesh.position.push_back(t->layer);

				mesh.position.push_back(t->offset.x);
				mesh.position.push_back(t->offset.y + t->extent.y);
				mesh.position.push_back(t->layer);
			}
			else {

				mesh.position.push_back(t->offset.x + t->extent.x);
				mesh.position.push_back(t->offset.y);
				mesh.position.push_back(t->layer);

				mesh.position.push_back(t->offset.x + t->extent.x);
				mesh.position.push_back(t->offset.y + t->extent.y);
				mesh.position.push_back(t->layer);

				mesh.position.push_back(t->offset.x);
				mesh.position.push_back(t->offset.y + t->extent.y);
				mesh.position.push_back(t->layer);

				mesh.position.push_back(t->offset.x);
				mesh.position.push_back(t->offset.y);
				mesh.position.push_back(t->layer);

			}

			index += 4;
		}

		model.addData(mesh);

	}
	void TilemapAnim::drawMap()
	{
		g_TextureManager->bindTex(texRef);
		model.draw();
	}
	void TilemapAnim::loadTileFromJSON(std::string path)
	{
	}
}