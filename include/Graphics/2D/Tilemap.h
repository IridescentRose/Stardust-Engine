#pragma once
#include "SpriteAdvanced.h"

namespace Stardust::Graphics::Render2D {
	struct Tile {
		glm::vec2 offset;
		glm::vec2 extent;

		short texIndex;
		unsigned int rgba;
	};

	class Tilemap {
	public:
		Tilemap(TextureAtlas* atlas, Texture* texture);

		int addTile(Tile* t);
		void updateTile(int tileNum, Tile* t);
		void clearTiles();

		void buildMap();

		void drawMap();

	private:
		TextureAtlas* m_Atlas;
		std::vector<Tile*> m_Tiles;
		std::vector<Vertex> vertices;
		Texture* texRef;


	};

}