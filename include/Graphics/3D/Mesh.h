#pragma once
#include "../RenderTypes.h"
using namespace Stardust::Graphics;

namespace Stardust::Graphics::Render3D {
	class Mesh {
	public:

		void addVertex(Stardust::Graphics::Vertex v);
		void clearMesh();

		size_t getSize();
		void* getData();

		void drawMesh();

	private:
		std::vector<Vertex> vertices;
	};

	class UntexturedMesh {
	public:

		void addVertex(Stardust::Graphics::ColorVertex v);
		void clearMesh();

		size_t getSize();
		void* getData();

		void drawMesh();

	private:
		std::vector<ColorVertex> vertices;
	};


}