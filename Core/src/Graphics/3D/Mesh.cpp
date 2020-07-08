#include <Graphics/3D/Mesh.h>

namespace Stardust::Graphics::Render3D {

	void Mesh::addVertex(Stardust::Graphics::Vertex v)
	{
		vertices.push_back(v);
	}

	void Mesh::clearMesh()
	{
		vertices.clear();
	}

	size_t Mesh::getSize()
	{
		return vertices.size();
	}

	void* Mesh::getData()
	{
		return vertices.data();
	}

	void Mesh::drawMesh()
	{
		sceGuEnable(GU_TEXTURE_2D);
		sceGuFrontFace(GU_CCW);
		sceGumDrawArray(GU_TRIANGLES, GU_VERTEX_32BITF | GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, getSize(), nullptr, getData());
	}


	void UntexturedMesh::addVertex(Stardust::Graphics::ColorVertex v)
	{
		vertices.push_back(v);
	}

	void UntexturedMesh::clearMesh()
	{
		vertices.clear();
	}

	size_t UntexturedMesh::getSize()
	{
		return vertices.size();
	}

	void* UntexturedMesh::getData()
	{
		return vertices.data();
	}

	void UntexturedMesh::drawMesh()
	{
		sceGuFrontFace(GU_CCW);
		sceGumDrawArray(GU_TRIANGLES, GU_VERTEX_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, getSize(), nullptr, getData());
	}

}