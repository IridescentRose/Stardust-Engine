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

	inline bool collided2D(const AABB2D& aabb, const vec2& v) {
		return (v.x > aabb.offset.x && v.x < (aabb.extent + aabb.offset).x && v.y > aabb.offset.y&& v.y < (aabb.extent + aabb.offset).y);
	}
	inline bool collided(const AABB& aabb, const vec3& v) {
		return (v.x > aabb.offset.x&& v.x < (aabb.extent + aabb.offset).x && v.y > aabb.offset.y&& v.y < (aabb.extent + aabb.offset).y && v.z > aabb.offset.z&& v.z < (aabb.extent + aabb.offset).z);
	}

	inline bool AABBIntersect2D(const AABB2D& a, const AABB2D& b) {
		//∀i∈x,y
		if ((a.offset.x > (b.offset + b.extent).x || b.offset.x > (a.offset + a.extent).x) && (a.offset.y > (b.offset + b.extent).y || b.offset.y > (a.offset + a.extent).y)) {
			return false;
		}
		else {
			return true;
		}
	}

	inline bool AABBIntersect3D(const AABB& a, const AABB& b) {
		//∀i∈x,y
		if ((a.offset.x > (b.offset + b.extent).x || b.offset.x > (a.offset + a.extent).x) && (a.offset.y > (b.offset + b.extent).y || b.offset.y > (a.offset + a.extent).y) && (a.offset.z > (b.offset + b.extent).z || b.offset.z > (a.offset + a.extent).z)) {
			return false;
		}
		else {
			return true;
		}
	}
}