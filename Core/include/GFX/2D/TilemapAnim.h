#pragma once
#include "Sprite.h"
#include "Tilemap.h"

#include <Utilities/Logger.h>

namespace Stardust::GFX::Render2D {

	struct TileAnim : public Tile {
		bool isAnim;
		short indexStart;
		char animLength;
		char tickNumber;
	};

	class TilemapAnim {
	public:
		TilemapAnim(TextureAtlas* atlas, unsigned int texture);
		~TilemapAnim();

		int addTile(TileAnim* t);
		void updateTile(int tileNum, TileAnim* t);
		void tickPhase();
		void clearTiles();

		void setAmbientLight(AmbientLight light);
		void addPointLight(PointLight light);
		void calculateLighting();

		void setPosition(glm::vec2 pos);

		bool checkPhysics(const Math::AABB2D& aabb);

		void buildMap();

		void drawMap();

		void loadTileFromJSON(std::string path);
	private:
		TextureAtlas* m_Atlas;
		std::vector<TileAnim*> m_Tiles;
		Mesh mesh;
		Model model;

		glm::vec2 position;
		unsigned int texRef;


		AmbientLight ambient;
		std::vector<PointLight> pointLights;
	};

}