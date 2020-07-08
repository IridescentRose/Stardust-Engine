#pragma once

#include <array>
#include <Math/Matrix.h>
#include <Math/AABB.h>

namespace Stardust::Math {

    struct Plane {
        float distanceToPoint(const vec3& point) const;

        float distanceToOrigin;
        vec3 normal;
    };

    class ViewFrustum {
    public:
        void update(const mat4& projViewMatrix) noexcept;

        bool isBoxInFrustum(const AABB& box) const noexcept;

    private:
        std::array<Plane, 6> m_planes;
    };
}