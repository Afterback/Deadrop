#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

namespace deadrop::math
{
    // defines for PI values
    constexpr float PI = static_cast<float>(M_PI);
    constexpr float PI_HALF = static_cast<float>(M_PI_2);
    constexpr float PI_QUARTER = static_cast<float>(M_PI_4);

    // converts an angle in degrees into radians
    constexpr float ToRadians(float angle_in_degrees)
    {
        return angle_in_degrees * (PI / 180.0f);
    }

    // converts an angle in radians into degrees
    constexpr float ToDegrees(float angle_in_radians)
    {
        return angle_in_radians * (180.0f / PI);
    }

    // returns the sine of x
    template<class T>
    inline T Sin(T x)
    {
        return static_cast<T>(sin(x));
    }

    // returns the sine of x
    // NOTE: this is a float type specialization
    template<>
    inline float Sin<float>(float x)
    {
        return sinf(x);
    }

    // returns the cosine of x
    template<class T>
    inline T Cos(T x)
    {
        return static_cast<T>(cos(x));
    }

    // returns the cosine of x
    // NOTE: this is a float type specialization
    template<>
    inline float Cos(float x)
    {
        return cosf(x);
    }

    // returns the tangent of x
    template<class T>
    inline T Tan(T x)
    {
        return static_cast<T>(tan(x));
    }

    // returns the tangent of x
    // NOTE: this is a float type specialization
    template<>
    inline float Tan(float x)
    {
        return tanf(x);
    }
}
