#pragma once
#include "engine/core/math/vec3.h"

namespace deadrop
{
    namespace math
    {
        class AABB
        {
        public:
            // default constructor
            AABB() : m_min(vec3f(0.0f)), m_max(vec3f(0.0f)) {}
            // constructors
            AABB(const vec3f& min, const vec3f& max) : m_min(min), m_max(max) {}

            // returns whether this AABB intersects with a specific AABB
            bool intersects(const AABB& aabb) const;

        private:
            vec3f m_min;
            vec3f m_max;
        };

        bool AABB::intersects(const AABB& aabb) const
        {
            if ((m_min.x <= aabb.m_min.x) && (m_max.x >= aabb.m_max.x) &&
                (m_min.y <= aabb.m_min.y) && (m_max.y >= aabb.m_max.y) &&
                (m_min.z <= aabb.m_min.z) && (m_max.z >= aabb.m_max.z))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}