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

        // returns the normalized vector of the passed vector parameter
        template<class T>
        constexpr vec3<T> Normalize(vec3<T> vec)
        {
            const T length = static_cast<T>(sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
            return vec3<T>
            {
                vec.x / length,
                vec.y / length,
                vec.z / length
            };
        }

        // returns the cross product of two vectors
        // NOTE: the cross product is a vector that is perpendicular to the plane 
        // that both vectors lay on, as such it is the normal of that plane
        template<class T>
        constexpr vec3<T> Cross(vec3<T> vec1, vec3<T> vec2)
        {
            // NOTE: the formula works with both left-handed and right-handed coordinates
            // when either are used, the formula will return the correct result for the input
            // in that same coordinate (left-handed input will return a lef-handed vector and
            // right-handed input will return a right-handed vector).
            // formula:
            // Ax = (By * Cz) - (Bz * Cy)
            // Ay = (Bz * Cx) - (Bx * Cz)
            // Az = (Bx * Cy) - (By * Cx)

            return vec3<T>
            {
                (vec1.y* vec2.z) - (vec1.z * vec2.y),
                (vec1.z* vec2.x) - (vec1.x * vec2.z),
                (vec1.x* vec2.y) - (vec1.y * vec2.x)
            };
        }

        // returns the dot product of two vectors
        template<class T>
        constexpr T Dot(vec3<T> vec1, vec3<T> vec2)
        {
            return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
        }

        // returns the same vector but with all the components being negative
        template<class T>
        constexpr vec3<T> Negate(vec3<T> vec)
        {
            return vec3<T>
            {
                -vec.x,
                -vec.y,
                -vec.z
            };
        }
    }

    // aliases
    using vec3f = math::vec3<float>;
    using vec3i = math::vec3<i32>;
    using vec3u = math::vec3<u32>;
}