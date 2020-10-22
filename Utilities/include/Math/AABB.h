#pragma once

#include <Math/Vector.h>

namespace Stardust::Math {
	/*Might belong in physics?*/

	struct AABB2D {
		vec2 offset; //Lower left corner
		vec2 extent; //Bounds
	};

	struct AABB {
		vec3 offset; //Lower Left Front Corner
		vec3 extent; //Bounds
	};

	inline auto collided2D(const AABB2D& aabb, const vec2& v) -> bool {
		return (v.x > aabb.offset.x && v.x < (aabb.extent + aabb.offset).x && v.y > aabb.offset.y&& v.y < (aabb.extent + aabb.offset).y);
	}
	inline auto collided(const AABB& aabb, const vec3& v) -> bool {
		return (v.x > aabb.offset.x&& v.x < (aabb.extent + aabb.offset).x && v.y > aabb.offset.y&& v.y < (aabb.extent + aabb.offset).y && v.z > aabb.offset.z&& v.z < (aabb.extent + aabb.offset).z);
	}

	inline auto AABBIntersect2D(const AABB2D& a, const AABB2D& b) -> bool {
		//∀i∈x,y
		return (a.offset.x <= (b.extent + b.offset).x && (a.extent + a.offset).x >= b.offset.x) &&
			(a.offset.y <= (b.extent + b.offset).y && (a.extent + a.offset).y >= b.offset.y);
	}

	inline auto AABBIntersect3D(const AABB& a, const AABB& b) -> bool {
		//∀i∈x,y
		return (a.offset.x <= (b.extent + b.offset).x && (a.extent + a.offset).x >= b.offset.x) &&
			(a.offset.y <= (b.extent + b.offset).y && (a.extent + a.offset).y >= b.offset.y) &&
			(a.offset.z <= (b.extent + b.offset).z && (a.extent + a.offset).z >= b.offset.z);
	}
}
