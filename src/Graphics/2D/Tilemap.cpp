#include <Graphics/2D/Tilemap.h>
#include <Utilities/Logger.h>
#include <Utilities/JSON.h>

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
				Math::AABB2D b = { {t->offset.x + t->extent.x / 2.0f, t->offset.y}, t->extent };
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

	Tile* parseTile(Json::Value v) {
		Tile* res = new Tile();

		res->offset = { 0, 0 };
		res->extent = { v["size"][0].asFloat(), v["size"][1].asFloat() };
		res->texIndex = v["index"].asInt();
		res->layer = v["layer"].asInt();
		res->rgba = 0xFFFFFFFF;
		res->rotation = v["rotation"].asInt();
		res->physics = v["physics"].asBool();

		return res;
	}

	void Tilemap::loadTileFromJSON(std::string path)
	{
		Json::Value index;
		std::fstream file(path);

		std::string str;
		if (file.is_open()) {

			file >> index;
			file.close();

			if (index.size() > 0) {
				std::string type = index["type"].asString();
				if (type == "regular") {
					std::map<int, Tile*> keyMap;

					for (int i = 0; i < index["keyMap"].size(); i++) {
						Json::Value val = index["keyMap"][i];

						keyMap.emplace(val["key"].asInt(), parseTile(val));
					}

					int width = index["properties"]["width"].asInt();
					int height = index["properties"]["height"].asInt();
					int tileSize = index["properties"]["tileSize"].asInt();

					Utilities::app_Logger->log("NUM TILES EXPECTED: " + std::to_string(width * height));
					Utilities::app_Logger->log("NUM TILES RECEIVED: " + std::to_string(index["tileMap"].size()));

					if (index["tileMap"].size() == width * height) {
						for (int i = 0; i < index["tileMap"].size(); i++) {
							int x = i % width;
							int y = i / width;
							x *= tileSize;
							y *= tileSize;


							Tile temp = *keyMap[index["tileMap"][i].asInt()];

							Tile* tile = new Tile();
							
							tile->offset = { x, y };
							tile->extent = temp.extent * 1.01f;
							tile->texIndex = temp.texIndex;
							tile->layer = temp.layer;
							tile->rgba = temp.rgba;
							tile->rotation = temp.rotation;
							tile->physics = temp.physics;

							addTile(tile);
						}
					}

				}
			}

		}
	}
}