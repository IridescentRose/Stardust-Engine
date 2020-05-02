#include <Graphics/2D/Tilemap.h>
#include <Utilities/Logger.h>

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

			if(t->rotation == 0 || t->rotation == 4){
				vertices.push_back({ coords[(0 + t->rotation) % 8], coords[(1 + t->rotation) % 8], t->rgba, t->offset.x, t->offset.y , t->layer });
				vertices.push_back({ coords[(4 + t->rotation) % 8], coords[(5 + t->rotation) % 8], t->rgba, t->offset.x + t->extent.x, t->offset.y + t->extent.y, t->layer });
			}
			else {
				vertices.push_back({ coords[(0 + t->rotation) % 8], coords[(1 + t->rotation) % 8], t->rgba, t->offset.x + t->extent.x, t->offset.y , t->layer });
				vertices.push_back({ coords[(4 + t->rotation) % 8], coords[(5 + t->rotation) % 8], t->rgba, t->offset.x, t->offset.y + t->extent.y, t->layer });
			}
		}
	}

	bool Tilemap::checkPhysics(const Math::AABB2D& aabb)
	{
		for (auto t : m_Tiles) {
			if (t->physics) {
				Math::AABB2D b = { {t->offset.x + t->extent.x / 4.0f, t->offset.y}, t->extent };
				bool res = Math::AABBIntersect2D(aabb, b);

				if (res)
					return res;
			}
		}
		return false;
	}

	void Tilemap::drawMap()
	{
		sceGuEnable(GU_BLEND);
		sceGuEnable(GU_TEXTURE_2D);

		texRef->bindTexture(0, 0, true);

		sceGumDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, vertices.size(), 0, vertices.data());

		sceGuDisable(GU_BLEND);
	}
	void Tilemap::loadTileFromJSON(std::string path)
	{
	}
}