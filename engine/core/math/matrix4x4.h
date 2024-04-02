#pragma once

namespace deadrop::math
{
    class Matrix4x4
    {
    public:
        // default constructor
        // all elements are zeroed
        Matrix4x4();

        // constructs a matrix by specifying
        // all its elements (rows and columns)
        Matrix4x4(
            float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);

        // constructs a matrix by specifying the diagnal values only, the rest is zeroed
        Matrix4x4(float m00, float m11, float m22, float m33);

        // returns the transpose of the matrix
        Matrix4x4 GetTranspose() const;

        // operators
        Matrix4x4 operator*(const Matrix4x4& param);
        Matrix4x4 operator+(const Matrix4x4& param);

        // returns a reference to a single element
        // in the matrix at a specific location
        float& operator ()(size_t i, size_t j);

        // returns the identity matrix
        static Matrix4x4 Identity();

    private:
        float mat[4][4];
    };

    // alias
    using mat4f = Matrix4x4;
}
