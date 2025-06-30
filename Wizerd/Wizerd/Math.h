#pragma once

#include <cstring>
#include <initializer_list>
#include <stdexcept>
#include <cmath>

class Matrix4x4
{
public:
  float m[4][4];

  Matrix4x4() { std::memset(m, 0, sizeof(m)); }

  static Matrix4x4 Identity()
  {
    Matrix4x4 mat;
    for (int i = 0; i < 4; ++i)
      mat.m[i][i] = 1.0f;
    return mat;
  }

  Matrix4x4(std::initializer_list<float> list)
  {
    if (list.size() != 16)
      throw std::invalid_argument("Matrix4x4 requires 16 elements.");
    auto it = list.begin();
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        m[i][j] = *it++;
  }

  Matrix4x4 operator*(const Matrix4x4& rhs) const
  {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        for (int k = 0; k < 4; ++k)
          result.m[i][j] += m[i][k] * rhs.m[k][j];
    return result;
  }

  Matrix4x4 Transpose() const
  {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        result.m[j][i] = m[i][j];
    return result;
  }

  float* operator[](int row) { return m[row]; }
  const float* operator[](int row) const { return m[row]; }

  // ‹ts—ñ‚ðŒvŽZ‚·‚é
  Matrix4x4 Inverse() const
  {
    Matrix4x4 inv;
    const float* a = &m[0][0];
    float* o = &inv.m[0][0];

    o[0] = a[5] * a[10] * a[15] -
      a[5] * a[11] * a[14] -
      a[9] * a[6] * a[15] +
      a[9] * a[7] * a[14] +
      a[13] * a[6] * a[11] -
      a[13] * a[7] * a[10];

    o[4] = -a[4] * a[10] * a[15] +
      a[4] * a[11] * a[14] +
      a[8] * a[6] * a[15] -
      a[8] * a[7] * a[14] -
      a[12] * a[6] * a[11] +
      a[12] * a[7] * a[10];

    o[8] = a[4] * a[9] * a[15] -
      a[4] * a[11] * a[13] -
      a[8] * a[5] * a[15] +
      a[8] * a[7] * a[13] +
      a[12] * a[5] * a[11] -
      a[12] * a[7] * a[9];

    o[12] = -a[4] * a[9] * a[14] +
      a[4] * a[10] * a[13] +
      a[8] * a[5] * a[14] -
      a[8] * a[6] * a[13] -
      a[12] * a[5] * a[10] +
      a[12] * a[6] * a[9];

    o[1] = -a[1] * a[10] * a[15] +
      a[1] * a[11] * a[14] +
      a[9] * a[2] * a[15] -
      a[9] * a[3] * a[14] -
      a[13] * a[2] * a[11] +
      a[13] * a[3] * a[10];

    o[5] = a[0] * a[10] * a[15] -
      a[0] * a[11] * a[14] -
      a[8] * a[2] * a[15] +
      a[8] * a[3] * a[14] +
      a[12] * a[2] * a[11] -
      a[12] * a[3] * a[10];

    o[9] = -a[0] * a[9] * a[15] +
      a[0] * a[11] * a[13] +
      a[8] * a[1] * a[15] -
      a[8] * a[3] * a[13] -
      a[12] * a[1] * a[11] +
      a[12] * a[3] * a[9];

    o[13] = a[0] * a[9] * a[14] -
      a[0] * a[10] * a[13] -
      a[8] * a[1] * a[14] +
      a[8] * a[2] * a[13] +
      a[12] * a[1] * a[10] -
      a[12] * a[2] * a[9];

    o[2] = a[1] * a[6] * a[15] -
      a[1] * a[7] * a[14] -
      a[5] * a[2] * a[15] +
      a[5] * a[3] * a[14] +
      a[13] * a[2] * a[7] -
      a[13] * a[3] * a[6];

    o[6] = -a[0] * a[6] * a[15] +
      a[0] * a[7] * a[14] +
      a[4] * a[2] * a[15] -
      a[4] * a[3] * a[14] -
      a[12] * a[2] * a[7] +
      a[12] * a[3] * a[6];

    o[10] = a[0] * a[5] * a[15] -
      a[0] * a[7] * a[13] -
      a[4] * a[1] * a[15] +
      a[4] * a[3] * a[13] +
      a[12] * a[1] * a[7] -
      a[12] * a[3] * a[5];

    o[14] = -a[0] * a[5] * a[14] +
      a[0] * a[6] * a[13] +
      a[4] * a[1] * a[14] -
      a[4] * a[2] * a[13] -
      a[12] * a[1] * a[6] +
      a[12] * a[2] * a[5];

    o[3] = -a[1] * a[6] * a[11] +
      a[1] * a[7] * a[10] +
      a[5] * a[2] * a[11] -
      a[5] * a[3] * a[10] -
      a[9] * a[2] * a[7] +
      a[9] * a[3] * a[6];

    o[7] = a[0] * a[6] * a[11] -
      a[0] * a[7] * a[10] -
      a[4] * a[2] * a[11] +
      a[4] * a[3] * a[10] +
      a[8] * a[2] * a[7] -
      a[8] * a[3] * a[6];

    o[11] = -a[0] * a[5] * a[11] +
      a[0] * a[7] * a[9] +
      a[4] * a[1] * a[11] -
      a[4] * a[3] * a[9] -
      a[8] * a[1] * a[7] +
      a[8] * a[3] * a[5];

    o[15] = a[0] * a[5] * a[10] -
      a[0] * a[6] * a[9] -
      a[4] * a[1] * a[10] +
      a[4] * a[2] * a[9] +
      a[8] * a[1] * a[6] -
      a[8] * a[2] * a[5];

    float det = a[0] * o[0] + a[1] * o[4] + a[2] * o[8] + a[3] * o[12];
    if (std::fabs(det) < 1e-6f)
      return Matrix4x4::Identity(); // ‹ts—ñ‚ª‘¶Ý‚µ‚È‚¢ê‡‚Í’PˆÊs—ñ‚ð•Ô‚·

    float invDet = 1.0f / det;
    for (int i = 0; i < 16; ++i)
      o[i] *= invDet;

    return inv;
  }
};
