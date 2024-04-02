#pragma once
#include "math.h"

namespace deadrop::math
{
    // returns an ortographic projection matrix that is off center
    mat4f ComposeOrthoOffCenterLH(
        float left, float right,
        float bottom, float top,
        float nearZ, float farZ);

    // returns an ortographic projection matrix
    mat4f ComposeOrthoLH(
        float viewWidth, float viewHeight,
        float nearZ, float farZ);

    // returns a view matrix based on the lookat parameters
    mat4f ComposeLookAtLH(vec3f eye, vec3f at, vec3f up);

    // returns a left-handed projection matrix
    // based on the field-of-view and near and far clipping planes
    mat4f ComposeProjectionFovLH(
        float fovAngleY, float aspectRatio,
        float nearZ, float farZ);

    // returns a left-handed infinite projection matrix
    // based on the field-of-view and just a near clipping plane
    mat4f ComposeInfiniteProjectionFovLH(
        float fovAngleY, float aspectRatio, float nearZ);
}
