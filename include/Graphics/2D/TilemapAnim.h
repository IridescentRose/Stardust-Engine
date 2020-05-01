#pragma once
#include "SpriteAdvanced.h"
#include "Tilemap.h"

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

		int addTile(TileAnim* t);
		void updateTile(int tileNum, TileAnim* t);
		void tickPhase();
		void clearTiles();

		void buildMap();

		void drawMap();

	private:
		TextureAtlas* m_Atlas;
		std::vector<TileAnim*> m_Tiles;
		std::vector<Vertex> vertices;
		Texture* texRef;


	};

}