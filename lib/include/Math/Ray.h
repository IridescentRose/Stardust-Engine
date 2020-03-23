#pragma once

#include <Math/AABB.h>

namespace Stardust::Math {

	class Ray2D {
	public:
		Ray2D(vec2 position, vec2 direction) : m_pos(position), m_dir(direction) {
			m_newPos = m_pos;
		}

		//Number of steps
		inline void step(float multiplier) {
			m_newPos += m_dir * multiplier;
		}

		inline vec2 getEnd() {
			return m_newPos;
		}

	private:
		vec2 m_pos, m_dir;
		vec2 m_newPos;

	};


	class Ray {
	public:
		Ray(vec3 position, vec3 direction) : m_pos(position), m_dir(direction) {
			m_newPos = m_pos;
		}

		//Number of steps
		inline void step(float multiplier) {
			m_newPos += m_dir * multiplier;
		}

		inline vec3 getEnd() {
			return m_newPos;
		}

	private:
		vec3 m_pos, m_dir;
		vec3 m_newPos;

	};
}