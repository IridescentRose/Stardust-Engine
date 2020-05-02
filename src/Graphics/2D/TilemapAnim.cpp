#include <Graphics/2D/TilemapAnim.h>

namespace Stardust::Graphics::Render2D {
	TilemapAnim::TilemapAnim(TextureAtlas* atlas, Texture* texture)
	{
		m_Atlas = atlas;
		texRef = texture;

		vertices.clear();
		m_Tiles.clear();
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

			if (t->rotation == 0 || t->rotation == 4) {
				vertices[i*2] = { coords[(0 + t->rotation) % 8], coords[(1 + t->rotation) % 8], t->rgba, t->offset.x, t->offset.y , t->layer };
				vertices[i*2 + 1] = { coords[(4 + t->rotation) % 8], coords[(5 + t->rotation) % 8], t->rgba, t->offset.x + t->extent.x, t->offset.y + t->extent.y, t->layer };
			}
			else {
				vertices[i * 2] = { coords[(0 + t->rotation) % 8], coords[(1 + t->rotation) % 8], t->rgba, t->offset.x + t->extent.x, t->offset.y , t->layer };
				vertices[i * 2 + 1] = { coords[(4 + t->rotation) % 8], coords[(5 + t->rotation) % 8], t->rgba, t->offset.x, t->offset.y + t->extent.y, t->layer };
			}

		}
		sceKernelDcacheWritebackInvalidateAll();
	}

	bool TilemapAnim::checkPhysics(const Math::AABB2D& aabb)
	{
		for (auto t : m_Tiles) {

			if (t->physics) {
				Math::AABB2D b = { {t->offset.x, t->offset.y + 272}, t->extent };
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
	void TilemapAnim::buildMap()
	{
		vertices.clear();
		for (auto t : m_Tiles) {
			std::array<float, 8> coords;
			if (t->isAnim) {
				coords = m_Atlas->getTexture(t->indexStart + t->tickNumber);
			}
			else {
				coords = m_Atlas->getTexture(t->texIndex);
			}

			if (t->rotation == 0 || t->rotation == 4) {
				vertices.push_back({ coords[(0 + t->rotation) % 8], coords[(1 + t->rotation) % 8], t->rgba, t->offset.x, t->offset.y , t->layer });
				vertices.push_back({ coords[(4 + t->rotation) % 8], coords[(5 + t->rotation) % 8], t->rgba, t->offset.x + t->extent.x, t->offset.y + t->extent.y, t->layer });
			}
			else {
				vertices.push_back({ coords[(0 + t->rotation) % 8], coords[(1 + t->rotation) % 8], t->rgba, t->offset.x + t->extent.x, t->offset.y , t->layer });
				vertices.push_back({ coords[(4 + t->rotation) % 8], coords[(5 + t->rotation) % 8], t->rgba, t->offset.x, t->offset.y + t->extent.y, t->layer });
			}
		}
	}
	void TilemapAnim::drawMap()
	{
		sceGuEnable(GU_BLEND);
		sceGuEnable(GU_TEXTURE_2D);

		texRef->bindTexture(0, 0, true);

		sceGumDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, vertices.size(), 0, vertices.data());

		sceGuDisable(GU_BLEND);
	}
}