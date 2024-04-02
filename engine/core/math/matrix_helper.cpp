#include "matrix_helper.h"
#include "scalar.h"
using namespace deadrop::math;

mat4f deadrop::math::ComposeOrthoOffCenterLH(
    float left, float right,
    float bottom, float top,
    float nearZ, float farZ)
{
    mat4f orthoLH =
    {
        2.0f / (right - left), 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
        0.0, 0.0, 1.0f / (nearZ - farZ), 0.0f,
        (left + right) / (left - right), (top + bottom) / (bottom - top), nearZ / (nearZ - farZ), 1.0f
    };
    return orthoLH;
}

mat4f deadrop::math::ComposeOrthoLH(
    float viewWidth, float viewHeight,
    float nearZ, float farZ)
{
    float fRange = 1.0f / (farZ / nearZ);
    mat4f orthoLH =
    {
        2.0f / viewWidth, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / viewHeight, 0.0f, 0.0f,
        0.0f, 0.0f, fRange, 0.0f,
        0.0f, 0.0f, -fRange * nearZ, 1.0f
    };
    return orthoLH;
}

mat4f deadrop::math::ComposeLookAtLH(vec3f eye, vec3f at, vec3f up)
{
    vec3f zaxis = Normalize(at - eye);
    vec3f xaxis = Normalize(Cross(up, zaxis));
    vec3f yaxis = Cross(zaxis, xaxis);
    vec3f negateEye = Negate(eye);

    return mat4f
    {
        xaxis.x, yaxis.x, zaxis.x, 0.0f,
        xaxis.y, yaxis.y, zaxis.y, 0.0f,
        xaxis.z, yaxis.z, zaxis.z, 0.0f,
        Dot(xaxis, negateEye), Dot(yaxis, negateEye), Dot(zaxis, negateEye), 1.0f
    };
}

mat4f deadrop::math::ComposeProjectionFovLH(
    float fovAngleY,
    float aspectRatio,
    float nearZ, float farZ)
{
    float sinFov = Sin(0.5f * fovAngleY);
    float cosFov = Cos(0.5f * fovAngleY);

    float height = cosFov / sinFov;
    float width = height / aspectRatio;
    float range = farZ / (farZ - nearZ);

    mat4f projectionFovLH =
    {
        width, 0.0f, 0.0f, 0.0f,
        0.0f, height, 0.0f, 0.0f,
        0.0f, 0.0f, range, 1.0f,
        0.0f, 0.0f, -range * nearZ, 0.0f
    };
    return projectionFovLH;
}

mat4f deadrop::math::ComposeInfiniteProjectionFovLH(
    float fovAngleY,
    float aspectRatio,
    float nearZ)
{
    float sinFov = Sin(0.5f * fovAngleY);
    float cosFov = Cos(0.5f * fovAngleY);

    float height = cosFov / sinFov;
    float width = height / aspectRatio;

    mat4f infProjectionFovLH =
    {
        width, 0.0f, 0.0f, 0.0f,
        0.0f, height, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -nearZ, 0.0f
    };
    return infProjectionFovLH;
}
