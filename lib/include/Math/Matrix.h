#pragma once

#include <Math/Vector.h>
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Stardust::Math {
	using glm::mat4;

	inline mat4 makeViewMatrix(const vec3& position, const vec3& rotation) {
		mat4 res = mat4(1.0);

		res = glm::rotate(res, rotation.x, { 1, 0, 0 });
		res = glm::rotate(res, rotation.y, { 0, 1, 0 });
		res = glm::rotate(res, rotation.z, { 0, 0, 1 });

		res = glm::translate(res, position);
		return res;
	}

	inline mat4 makeTransformMatrix(const vec3& position, const vec3& rotation, const vec3& scale) {
		mat4 res = mat4(1.0);

		res = glm::scale(res, scale);

		res = glm::rotate(res, rotation.x, { 1, 0, 0 });
		res = glm::rotate(res, rotation.y, { 0, 1, 0 });
		res = glm::rotate(res, rotation.z, { 0, 0, 1 });

		res = glm::translate(res, position);
		return res;
	}

	inline mat4 makePerspective(float fovy, float aspectRatio, float near, float far) {
		return glm::perspective(fovy, aspectRatio, near, far);
	}
	
	inline mat4 makeOrtho(float left, float right, float top, float bottom, float near, float far) {
		return glm::ortho(left, right, top, bottom, near, far);
	}
}