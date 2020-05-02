#include "..\..\..\include\Graphics\2D\SpriteAdvanced.h"

Stardust::Graphics::Render2D::Sprite2::Sprite2()
{
}

Stardust::Graphics::Render2D::Sprite2::~Sprite2()
{
	free(vertices);
}

Stardust::Graphics::Render2D::Sprite2::Sprite2(Texture* tex)
{
	vertices = (TextureVertex*)memalign(16, 2 * sizeof(TextureVertex));

	extent = { tex->width / 2.0f, tex->height / 2.0f };
	offset = { 0, 0 };
	scaleFactor = { 1, 1 };

	texRef = tex;

	vertices[0].x = offset.x - extent.x;
	vertices[0].y = offset.y - extent.y;
	vertices[0].z = 0;
	vertices[0].u = 0;
	vertices[0].v = 0;

	vertices[1].x = offset.x + extent.x;
	vertices[1].y = offset.y + extent.y;
	vertices[1].z = 0;
	vertices[1].u = 1;
	vertices[1].v = 1;

	sceKernelDcacheWritebackInvalidateRange(vertices, 2 * sizeof(TextureVertex));

	rgba = 0xFFFFFFFF;
	layer = 0;
	physics = false;
}

Stardust::Graphics::Render2D::Sprite2::Sprite2(Texture* tex, glm::vec2 size)
{
	vertices = (TextureVertex*)memalign(16, 2 * sizeof(TextureVertex));

	extent = size / 2.0f;
	offset = { 0, 0 };
	scaleFactor = { 1, 1 };

	texRef = tex;

	vertices[0].x = offset.x - extent.x;
	vertices[0].y = offset.y - extent.y;
	vertices[0].z = 0;
	vertices[0].u = 0;
	vertices[0].v = 0;

	vertices[1].x = offset.x + extent.x;
	vertices[1].y = offset.y + extent.y;
	vertices[1].z = 0;
	vertices[1].u = 1;
	vertices[1].v = 1;

	sceKernelDcacheWritebackInvalidateRange(vertices, 2 * sizeof(TextureVertex));

	rgba = 0xFFFFFFFF;
	layer = 0;
	physics = false;
}

Stardust::Graphics::Render2D::Sprite2::Sprite2(Texture* tex, glm::vec2 pos, glm::vec2 endPos)
{
	vertices = (TextureVertex*)memalign(16, 2 * sizeof(TextureVertex));

	extent = (endPos - pos) / 2.0f;
	offset = pos / 2.0f;
	scaleFactor = { 1, 1 };

	texRef = tex;

	vertices[0].x = offset.x - extent.x;
	vertices[0].y = offset.y - extent.y;
	vertices[0].z = 0;
	vertices[0].u = 0;
	vertices[0].v = 0;

	vertices[1].x = offset.x + extent.x;
	vertices[1].y = offset.y + extent.y;
	vertices[1].z = 0;
	vertices[1].u = 1;
	vertices[1].v = 1;

	sceKernelDcacheWritebackInvalidateRange(vertices, 2 * sizeof(TextureVertex));

	rgba = 0xFFFFFFFF;
	layer = 0;
	physics = false;
}

void Stardust::Graphics::Render2D::Sprite2::position(float x, float y)
{
	offset = { x / 2.0f, y / 2.0f};
	vertices[0].x = offset.x - (extent.x);
	vertices[0].y = offset.y - (extent.y);
	vertices[0].z = 0;
	vertices[0].u = 0;
	vertices[0].v = 0;

	vertices[1].x = offset.x + (extent.x);
	vertices[1].y = offset.y + (extent.y);
	vertices[1].z = 0;
	vertices[1].u = 1;
	vertices[1].v = 1;
	sceKernelDcacheWritebackInvalidateRange(vertices, 2 * sizeof(TextureVertex));
}

void Stardust::Graphics::Render2D::Sprite2::scale(float x, float y)
{
	scaleFactor = { x, y };
}

void Stardust::Graphics::Render2D::Sprite2::setColor(char r, char g, char b, char a)
{
	rgba = GU_RGBA(r, g, b, a);
}

void Stardust::Graphics::Render2D::Sprite2::setColor(int r)
{
	rgba = r;
}

void Stardust::Graphics::Render2D::Sprite2::setLayer(int l)
{
	layer = l;
}

void Stardust::Graphics::Render2D::Sprite2::setTexture(Texture* tex)
{
	texRef = tex;
}

void Stardust::Graphics::Render2D::Sprite2::setPhysics(bool p)
{
	physics = p;
}

void Stardust::Graphics::Render2D::Sprite2::draw()
{
	sceGumPushMatrix();
	sceGuEnable(GU_ALPHA_TEST);
	sceGuEnable(GU_BLEND);

	ScePspFVector3 scal = { scaleFactor.x, scaleFactor.y, 1.0f };
	sceGumScale(&scal);

	ScePspFVector3 loc = { offset.x, offset.y, layer };
	sceGumTranslate(&loc);

	sceGuEnable(GU_TEXTURE_2D);
	texRef->bindTexture(GU_NEAREST, GU_NEAREST, true);

	sceGuColor(rgba);

	sceGumDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 2, 0, vertices);

	sceGuDisable(GU_BLEND);
	sceGumPopMatrix();
}

bool Stardust::Graphics::Render2D::Sprite2::checkPhysics(const Math::AABB2D& aabb)
{
	if (physics) {
		Math::AABB2D b = { offset, extent };
		return Math::AABBIntersect2D(aabb, b);
	}
	return false;
}
