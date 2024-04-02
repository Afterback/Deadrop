#pragma once
#include "Engine/core/math/math.h"

namespace deadrop::math
{
    class Transform
    {
    public:
        // returns a translation matrix
        // NOTE: the returned matrix is row-major
        static mat4f MatrixTranslate(vec3f translation_vec);

        // returns a scale matrix
        // NOTE: the returned matrix is row-major
        static mat4f MatrixScale(vec3f scale_vec);

        // returns a rotation matrix around the x-axis, angle is in radians
        // NOTE: the returned matrix is row-major
        static mat4f MatrixRotateX(float angle);

        // returns a rotation matrix around the y-axis, angle is in radians
        // NOTE: the returned matrix is row-major
        static mat4f MatrixRotateY(float angle);

        // returns a rotation matrix around the z-axis, angle is in radians
        // NOTE: the returned matrix is row-major
        static mat4f MatrixRotateZ(float angle);
    };
}
