#pragma once

#include <Math/Vector.h>
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Stardust::Math {
	using glm::mat4;

	inline auto makeViewMatrix(const vec3& position, const vec3& rotation) -> mat4 {
		mat4 res = mat4(1.0);

		res = glm::rotate(res, rotation.x, { 1, 0, 0 });
		res = glm::rotate(res, rotation.y, { 0, 1, 0 });
		res = glm::rotate(res, rotation.z, { 0, 0, 1 });

		res = glm::translate(res, position);
		return res;
	}

	inline auto makeTransformMatrix(const vec3& position, const vec3& rotation, const vec3& scale) -> mat4 {
		mat4 res = mat4(1.0);

		res = glm::scale(res, scale);

		res = glm::rotate(res, rotation.x, { 1, 0, 0 });
		res = glm::rotate(res, rotation.y, { 0, 1, 0 });
		res = glm::rotate(res, rotation.z, { 0, 0, 1 });

		res = glm::translate(res, position);
		return res;
	}

#ifdef near
#undef near
#endif

#ifdef far
#undef far
#endif
	inline auto makePerspective(float fovy, float aspectRatio, float near, float far) -> mat4 {
		return glm::perspective(fovy, aspectRatio, near, far);
	}
	
	inline auto makeOrtho(float left, float right, float top, float bottom, float near, float far) -> mat4 {
		return glm::ortho(left, right, top, bottom, near, far);
	}
}
