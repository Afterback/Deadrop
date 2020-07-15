#pragma once
#include "engine/core/types.h"

namespace deadrop
{
    namespace math
    {
        template<typename T>
        class vec3
        {
        public:
            // constructors
            vec3() = default;
            constexpr vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
            constexpr vec3(T _xyz) : x(_xyz), y(_xyz), z(_xyz) {}

            // public vars
            T x;
            T y;
            T z;

            // operators
            vec3 operator*(T scalar)
            {
                return vec3
                {
                    x * scalar,
                    y * scalar,
                    z * scalar
                };
            }

            vec3 operator*(vec3 vec)
            {
                return vec3
                {
                    x * vec.x,
                    y * vec.y,
                    z * vec.z
                };
            }

            vec3 operator/(T scalar)
            {
                return vec3
                {
                    x / scalar,
                    y / scalar,
                    z / scalar
                };
            }

            vec3 operator/(vec3 vec)
            {
                return vec3
                {
                    x / vec.x,
                    y / vec.y,
                    z / vec.z
                };
            }

            vec3 operator+(T scalar)
            {
                return vec3
                {
                    x + scalar,
                    y + scalar,
                    z + scalar
                };
            }

            vec3 operator+(vec3 vec)
            {
                return vec3
                {
                    x + vec.x,
                    y + vec.y,
                    z + vec.z
                };
            }

            vec3 operator-(T scalar)
            {
                return vec3
                {
                    x - scalar,
                    y - scalar,
                    z - scalar
                };
            }

            vec3 operator-(vec3 vec)
            {
                return vec3
                {
                    x - vec.x,
                    y - vec.y,
                    z - vec.z
                };
            }
        };
    }

    // aliases
    using vec3f = math::vec3<float>;
    using vec3i = math::vec3<i32>;
    using vec3u = math::vec3<u32>;
}