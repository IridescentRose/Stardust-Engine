#include <Graphics/2D/Tilemap.h>

namespace Stardust::Graphics::Render2D {
	Tilemap::Tilemap(TextureAtlas* atlas, Texture* texture)
	{
		m_Atlas = atlas;
		texRef = texture;

		vertices.clear();
		m_Tiles.clear();
	}
	int Tilemap::addTile(Tile* t)
	{
		int idx = m_Tiles.size();
		m_Tiles.push_back(t);
		return idx;
	}

	void Tilemap::updateTile(int tileNum, Tile* t)
	{
		delete m_Tiles[tileNum];
		m_Tiles[tileNum] = t;
	}
	void Tilemap::clearTiles()
	{
		for (auto t : m_Tiles) {
			delete t;
		}
		m_Tiles.clear();
	}
	void Tilemap::buildMap()
	{
		vertices.clear();
		for (auto t : m_Tiles) {
			std::array<float, 8> coords = m_Atlas->getTexture(t->texIndex);

			vertices.push_back({ coords[0], coords[1], t->rgba, t->offset.x, t->offset.y , 0.f });
			vertices.push_back({ coords[4], coords[5], t->rgba, t->offset.x + t->extent.x, t->offset.y + t->extent.y, 0.f });
		}
	}

	void Tilemap::drawMap()
	{
		sceGuEnable(GU_BLEND);
		sceGuEnable(GU_TEXTURE_2D);

		texRef->bindTexture(0, 0, true);
		sceGumDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, vertices.size(), 0, vertices.data());

		sceGuDisable(GU_BLEND);
	}
}