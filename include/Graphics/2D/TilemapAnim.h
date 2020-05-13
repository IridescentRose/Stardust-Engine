#pragma once
#include "SpriteAdvanced.h"
#include "Tilemap.h"

#include <Utilities/Logger.h>

namespace Stardust::Graphics::Render2D {

	struct TileAnim : public Tile {
		bool isAnim;
		short indexStart;
		char animLength;
		char tickNumber;
	};

	class TilemapAnim {
	public:
		TilemapAnim(TextureAtlas* atlas, Texture* texture);
		~TilemapAnim();

		int addTile(TileAnim* t);
		void updateTile(int tileNum, TileAnim* t);
		void tickPhase();
		void clearTiles();

		void setPosition(glm::vec2 pos);

		bool checkPhysics(const Math::AABB2D& aabb);

		void buildMap();

		void drawMap();

		void loadTileFromJSON(std::string path);
	private:
		TextureAtlas* m_Atlas;
		std::vector<TileAnim*> m_Tiles;
		std::vector<Vertex> vertices;
		glm::vec2 position;
		Texture* texRef;


	};

}