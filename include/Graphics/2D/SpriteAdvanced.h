#pragma once
#include <Graphics/TextureUtil.h>
#include <Graphics/RenderTypes.h>
#include <Math/Vector.h>
#include <Math/AABB.h>

namespace Stardust::Graphics::Render2D {
	class Sprite2 {
	public:
		Sprite2();
		~Sprite2();
		Sprite2(Texture* tex);
		Sprite2(Texture* tex, glm::vec2 size);
		Sprite2(Texture* tex, glm::vec2 pos, glm::vec2 endPos);

		void position(float x, float y);
		void scale(float x, float y);

		void setColor(char r, char g, char b, char a);
		void setColor(int rgba);
		void setLayer(int layer);

		void setPhysics(bool p);

		void draw();


		bool checkPhysics(const Math::AABB2D& aabb);

	protected:
		glm::vec2 offset;
		glm::vec2 extent;
		glm::vec2 scaleFactor;

		Texture* texRef;
		bool physics;

		int rgba;
		int layer;
		TextureVertex* vertices;
	};
}