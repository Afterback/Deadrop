#pragma once

namespace deadrop
{
    namespace math
    {
        class Matrix4x4
        {
        public:
            // default constructor
            Matrix4x4()
            {
                // first row
                mat[0][0] = 0.0f; mat[0][1] = 0.0f; mat[0][2] = 0.0f; mat[0][3] = 0.0f;
                // second row
                mat[1][0] = 0.0f; mat[1][1] = 0.0f; mat[1][2] = 0.0f; mat[1][3] = 0.0f;
                // third row
                mat[2][0] = 0.0f; mat[2][1] = 0.0f; mat[2][2] = 0.0f; mat[2][3] = 0.0f;
                // fourth row
                mat[3][0] = 0.0f; mat[3][1] = 0.0f; mat[3][2] = 0.0f; mat[3][3] = 0.0f;
            }

            // constructors
            Matrix4x4(
                float m00, float m01, float m02, float m03,
                float m10, float m11, float m12, float m13,
                float m20, float m21, float m22, float m23,
                float m30, float m31, float m32, float m33
            )
            {
                // first row
                mat[0][0] = m00; mat[0][1] = m01; mat[0][2] = m02; mat[0][3] = m03;
                // second row
                mat[1][0] = m10; mat[1][1] = m11; mat[1][2] = m12; mat[1][3] = m13;
                // third row
                mat[2][0] = m20; mat[2][1] = m21; mat[2][2] = m22; mat[2][3] = m23;
                // fourth row
                mat[3][0] = m30; mat[3][1] = m31; mat[3][2] = m32; mat[3][3] = m33;
            }

            Matrix4x4(float m00, float m11, float m22, float m33)
            {
                // first row
                mat[0][0] = m00;  mat[0][1] = 0.0f; mat[0][2] = 0.0f; mat[0][3] = 0.0f;
                // second row
                mat[1][0] = 0.0f; mat[1][1] = m11; mat[1][2] = 0.0f; mat[1][3] = 0.0f;
                // third row
                mat[2][0] = 0.0f; mat[2][1] = 0.0f; mat[2][2] = m22; mat[2][3] = 0.0f;
                // fourth row
                mat[3][0] = 0.0f; mat[3][1] = 0.0f; mat[3][2] = 0.0f; mat[3][3] = m33;
            }

            // returns the transpose of the matrix
            Matrix4x4 GetTranspose()
            {
                Matrix4x4 temp = {};
                for (size_t i = 0; i < 4; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                    {
                        temp.mat[i][j] = mat[j][i];
                    }
                }
                return temp;
            }

            // operators
            Matrix4x4 operator*(const Matrix4x4& param)
            {
                Matrix4x4 temp = {};
                for (size_t i = 0; i < 4; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                    {
                        temp.mat[i][j] =
                            mat[i][0] * param.mat[0][j] +
                            mat[i][1] * param.mat[1][j] +
                            mat[i][2] * param.mat[2][j] +
                            mat[i][3] * param.mat[3][j];
                    }
                }
                return temp;
            }

            Matrix4x4 operator+(const Matrix4x4& param)
            {
                Matrix4x4 temp = {};
                for (size_t i = 0; i < 4; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                    {
                        temp.mat[i][j] = mat[i][j] + param.mat[i][j];
                    }
                }
                return temp;
            }

            // returns a reference to a single element in the matrix at a specific location
            float& operator ()(size_t i, size_t j)
            {
                // TODO: assert here to make sure we do not access out of bounds
                return mat[i][j];
            }

            // returns the identity matrix
            static Matrix4x4 Identity()
            {
                return Matrix4x4(1.0f, 1.0f, 1.0f, 1.0f);
            }

        private:
            float mat[4][4];
        };
    }

    // alias
    using mat4f = math::Matrix4x4;
}