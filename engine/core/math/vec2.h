#pragma once
#include "engine/core/types.h"

namespace deadrop::math
{
    template<typename T>
    class vec2
    {
    public:
        // constructors
        vec2() = default;
        constexpr vec2(T _x, T _y) : x(_x), y(_y) {}
        constexpr vec2(T _xy) : x(_xy), y(_xy) {}

        // public vars
        T x;
        T y;

        // operators
        vec2 operator*(T scalar)
        {
            return vec2
            {
                x * scalar,
                y * scalar
            };
        }

        vec2 operator*(vec2 vec)
        {
            return vec2
            {
                x * vec.x,
                y * vec.y
            };
        }

        vec2 operator/(T scalar)
        {
            return vec2
            {
                x / scalar,
                y / scalar
            };
        }

        vec2 operator/(vec2 vec)
        {
            return vec2
            {
                x / vec.x,
                y / vec.y
            };
        }

        vec2 operator+(T scalar)
        {
            return vec2
            {
                x + scalar,
                y + scalar
            };
        }

        vec2 operator+(vec2 vec)
        {
            return vec2
            {
                x + vec.x,
                y + vec.y
            };
        }

        vec2 operator-(T scalar)
        {
            return vec2
            {
                x - scalar,
                y - scalar
            };
        }

        vec2 operator-(vec2 vec)
        {
            return vec2
            {
                x - vec.x,
                y - vec.y
            };
        }
    };

    // aliases
    using vec2f = math::vec2<float>;
    using vec2i = math::vec2<i32>;
    using vec2u = math::vec2<u32>;
}
