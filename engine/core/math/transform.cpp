#include "transform.h"
#include "scalar.h"
using namespace deadrop::math;


mat4f Transform::MatrixTranslate(vec3f translation_vec)
{
    // compose a translation matrix
    mat4f translation_matrix =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        translation_vec.x, translation_vec.y, translation_vec.z, 1.0f
    };

    return translation_matrix;
}

mat4f Transform::MatrixScale(vec3f scale_vec)
{
    // compose a scale matrix
    mat4f scale_matrix =
    {
        scale_vec.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale_vec.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale_vec.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return scale_matrix;
}

mat4f Transform::MatrixRotateX(float angle)
{
    float sinAngle = math::Sin(angle);
    float cosAngle = math::Cos(angle);

    // compose a rotation matrix
    mat4f rotation_x_matrix =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosAngle, sinAngle, 0.0f,
        0.0f, -sinAngle, cosAngle, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return rotation_x_matrix;
}

mat4f Transform::MatrixRotateY(float angle)
{
    float sinAngle = math::Sin(angle);
    float cosAngle = math::Cos(angle);

    // compose a rotation matrix
    mat4f rotation_y_matrix =
    {
        cosAngle, 0.0f, -sinAngle, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinAngle, 0.0f, cosAngle, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return rotation_y_matrix;
}

mat4f Transform::MatrixRotateZ(float angle)
{
    float sinAngle = math::Sin(angle);
    float cosAngle = math::Cos(angle);

    // compose a 2d rotation matrix
    mat4f rotation_z_matrix =
    {
        cosAngle, sinAngle, 0.0f, 0.0f,
        -sinAngle, cosAngle, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return rotation_z_matrix;
}
