#pragma once
#include "engine/core/types.h"

namespace deadrop
{
    namespace math
    {
        template<typename T>
        class vec4
        {
        public:
            // constructors
            vec4() = default;
            constexpr vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
            constexpr vec4(T _xyzw) : x(_xyzw), y(_xyzw), z(_xyzw), w(_xyzw) {}

            // public vars
            T x;
            T y;
            T z;
            T w;

            // operators
            vec4 operator*(T scalar)
            {
                return vec4
                {
                    x * scalar,
                    y * scalar,
                    z * scalar,
                    w * scalar
                };
            }

            vec4 operator*(vec4 vec)
            {
                return vec4
                {
                    x * vec.x,
                    y * vec.y,
                    z * vec.z,
                    w * vec.w
                };
            }

            vec4 operator/(T scalar)
            {
                return vec4
                {
                    x / scalar,
                    y / scalar,
                    z / scalar,
                    w / scalar
                };
            }

            vec4 operator/(vec4 vec)
            {
                return vec4
                {
                    x / vec.x,
                    y / vec.y,
                    z / vec.z,
                    w / vec.w
                };
            }

            vec4 operator+(T scalar)
            {
                return vec4
                {
                    x + scalar,
                    y + scalar,
                    z + scalar,
                    w + scalar,
                };
            }

            vec4 operator+(vec4 vec)
            {
                return vec4
                {
                    x + vec.x,
                    y + vec.y,
                    z + vec.z,
                    w + vec.w,
                };
            }

            vec4 operator-(T scalar)
            {
                return vec4
                {
                    x - scalar,
                    y - scalar,
                    z - scalar,
                    w - scalar,
                };
            }

            vec4 operator-(vec4 vec)
            {
                return vec4
                {
                    x - vec.x,
                    y - vec.y,
                    z - vec.z,
                    w - vec.w,
                };
            }
        };
    }

    // aliases
    using vec4f = math::vec4<float>;
    using vec4i = math::vec4<i32>;
    using vec4u = math::vec4<u32>;
}