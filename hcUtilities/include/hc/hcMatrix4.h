#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcVector4.h"
#include "hc/hcVector3.h"

namespace hc
{
  /**
   * @brief 4x4 matrix class for linear algebra operations.
   *
   * Represents a 4x4 matrix with float components.
   *
   * Members can be accessed via m[4][4], m00/m01/.../m33, or as four Vector4f rows.
   */
  struct Matrix4
  {
    // Common matrices
    static constexpr Matrix4 Identity();
    static constexpr Matrix4 Zero();

    static constexpr Matrix4 Translate(float tx, float ty, float tz);
    static constexpr Matrix4 Translate(const Vector3f& translation);
    static constexpr Matrix4 Scale(float sx, float sy, float sz);
    static constexpr Matrix4 Scale(const Vector3f& scaleFactors);
    static Matrix4 Rotation(const Vector3f& eulerAngles);
    static Matrix4 RotationX(float angleRadians);
    static Matrix4 RotationY(float angleRadians);
    static Matrix4 RotationZ(float angleRadians);
    static Matrix4 LookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up);

    union
    {
      float m[4][4];

      struct
      {
        float m00, m01, m02, m03,
          m10, m11, m12, m13,
          m20, m21, m22, m23,
          m30, m31, m32, m33;
      };

      Vector4f mRow[4];
    };

    constexpr Matrix4();
    constexpr Matrix4(
      float m00, float m01, float m02, float m03,
      float m10, float m11, float m12, float m13,
      float m20, float m21, float m22, float m23,
      float m30, float m31, float m32, float m33
    );
    constexpr Matrix4(const Matrix4& other);

    constexpr Matrix4& operator=(const Matrix4& other);

    // Arithmetic operators
    constexpr Matrix4 operator+(const Matrix4& rhs) const;
    constexpr Matrix4 operator-(const Matrix4& rhs) const;
    constexpr Matrix4 operator*(const Matrix4& rhs) const;
    constexpr Vector4f operator*(const Vector4f& vec) const;
    constexpr Matrix4 operator*(float scalar) const;

    // Compound assignment
    constexpr Matrix4& operator+=(const Matrix4& rhs);
    constexpr Matrix4& operator-=(const Matrix4& rhs);
    constexpr Matrix4& operator*=(const Matrix4& rhs);
    constexpr Matrix4& operator*=(float scalar);

    // Equality
    constexpr bool operator==(const Matrix4& rhs) const;
    constexpr bool operator!=(const Matrix4& rhs) const;

    inline Matrix4 transposed() const;
    inline float determinant() const;
    inline Matrix4 inverted() const;
    inline Matrix4& transpose();
    inline Matrix4& invert();
  };

  // Static common matrices
  constexpr Matrix4 Matrix4::Identity()
  {
    return Matrix4(
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    );
  }

  constexpr Matrix4 Matrix4::Zero()
  {
    return Matrix4(
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f
    );
  }

  constexpr Matrix4 Matrix4::Translate(float tx, float ty, float tz)
  {
    return Matrix4(
      1.0f, 0.0f, 0.0f, tx,
      0.0f, 1.0f, 0.0f, ty,
      0.0f, 0.0f, 1.0f, tz,
      0.0f, 0.0f, 0.0f, 1.0f
    );
  }

  constexpr Matrix4 Matrix4::Translate(const Vector3f& translation)
  {
    return Translate(translation.x, translation.y, translation.z);
  }

  constexpr Matrix4 Matrix4::Scale(float sx, float sy, float sz)
  {
    return Matrix4(
      sx, 0.0f, 0.0f, 0.0f,
      0.0f, sy, 0.0f, 0.0f,
      0.0f, 0.0f, sz, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    );
  }

  constexpr Matrix4 Matrix4::Scale(const Vector3f& scaleFactors)
  {
    return Scale(scaleFactors.x, scaleFactors.y, scaleFactors.z);
  }

  inline Matrix4 Matrix4::Rotation(const Vector3f& eulerAngles)
  {
    Matrix4 rx = RotationX(eulerAngles.x);
    Matrix4 ry = RotationY(eulerAngles.y);
    Matrix4 rz = RotationZ(eulerAngles.z);

    return ry * rx * rz;
  }

  inline Matrix4 Matrix4::RotationX(float angleRadians)
  {
    float c = cos(angleRadians);
    float s = sin(angleRadians);

    return Matrix4(
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, c, -s, 0.0f,
      0.0f, s, c, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    );
  }

  inline Matrix4 Matrix4::RotationY(float angleRadians)
  {
    float c = cos(angleRadians);
    float s = sin(angleRadians);

    return Matrix4(
      c, 0.0f, s, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      -s, 0.0f, c, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    );
  }

  inline Matrix4 Matrix4::RotationZ(float angleRadians)
  {
    float c = cos(angleRadians);
    float s = sin(angleRadians);

    return Matrix4(
      c, -s, 0.0f, 0.0f,
      s, c, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    );
  }

  inline Matrix4 Matrix4::LookAt(
    const Vector3f& position,
    const Vector3f& target,
    const Vector3f& up
  )
  {
    Vector3f zaxis = (position - target).normalized();
    Vector3f xaxis = up.cross(zaxis).normalized();
    Vector3f yaxis = zaxis.cross(xaxis);

    return Matrix4(
      xaxis.x, yaxis.x, zaxis.x, position.x,
      xaxis.y, yaxis.y, zaxis.y, position.y,
      xaxis.z, yaxis.z, zaxis.z, position.z,
      0.0f, 0.0f, 0.0f, 1.0f
    ).inverted();
  }

  // Constructors
  constexpr Matrix4::Matrix4()
    : m00(0.0f), m01(0.0f), m02(0.0f), m03(0.0f),
    m10(0.0f), m11(0.0f), m12(0.0f), m13(0.0f),
    m20(0.0f), m21(0.0f), m22(0.0f), m23(0.0f),
    m30(0.0f), m31(0.0f), m32(0.0f), m33(0.0f)
  {
  }

  constexpr Matrix4::Matrix4(
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33
  )
    : m00(m00), m01(m01), m02(m02), m03(m03),
    m10(m10), m11(m11), m12(m12), m13(m13),
    m20(m20), m21(m21), m22(m22), m23(m23),
    m30(m30), m31(m31), m32(m32), m33(m33)
  {
  }

  constexpr Matrix4::Matrix4(const Matrix4& other)
    : m00(other.m00), m01(other.m01), m02(other.m02), m03(other.m03),
    m10(other.m10), m11(other.m11), m12(other.m12), m13(other.m13),
    m20(other.m20), m21(other.m21), m22(other.m22), m23(other.m23),
    m30(other.m30), m31(other.m31), m32(other.m32), m33(other.m33)
  {
  }

  constexpr Matrix4& Matrix4::operator=(const Matrix4& other)
  {
    m00 = other.m00; m01 = other.m01; m02 = other.m02; m03 = other.m03;
    m10 = other.m10; m11 = other.m11; m12 = other.m12; m13 = other.m13;
    m20 = other.m20; m21 = other.m21; m22 = other.m22; m23 = other.m23;
    m30 = other.m30; m31 = other.m31; m32 = other.m32; m33 = other.m33;
    return *this;
  }

  constexpr Matrix4 Matrix4::operator+(const Matrix4& rhs) const
  {
    return Matrix4(
      m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02, m03 + rhs.m03,
      m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
      m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
      m30 + rhs.m30, m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33
    );
  }

  constexpr Matrix4 Matrix4::operator-(const Matrix4& rhs) const
  {
    return Matrix4(
      m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02, m03 - rhs.m03,
      m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
      m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
      m30 - rhs.m30, m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33
    );
  }

  constexpr Matrix4 Matrix4::operator*(const Matrix4& rhs) const
  {
    return Matrix4(
      m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30,
      m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31,
      m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32,
      m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33,

      m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30,
      m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31,
      m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32,
      m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33,

      m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30,
      m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31,
      m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32,
      m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33,

      m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30,
      m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31,
      m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32,
      m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33
    );
  }

  constexpr Vector4f Matrix4::operator*(const Vector4f& vec) const
  {
    return Vector4f(
      m00 * vec.x + m01 * vec.y + m02 * vec.z + m03 * vec.w,
      m10 * vec.x + m11 * vec.y + m12 * vec.z + m13 * vec.w,
      m20 * vec.x + m21 * vec.y + m22 * vec.z + m23 * vec.w,
      m30 * vec.x + m31 * vec.y + m32 * vec.z + m33 * vec.w
    );
  }

  constexpr Matrix4 Matrix4::operator*(float scalar) const
  {
    return Matrix4(
      m00 * scalar, m01 * scalar, m02 * scalar, m03 * scalar,
      m10 * scalar, m11 * scalar, m12 * scalar, m13 * scalar,
      m20 * scalar, m21 * scalar, m22 * scalar, m23 * scalar,
      m30 * scalar, m31 * scalar, m32 * scalar, m33 * scalar
    );
  }

  constexpr Matrix4& Matrix4::operator+=(const Matrix4& rhs)
  {
    m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02; m03 += rhs.m03;
    m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13;
    m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23;
    m30 += rhs.m30; m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33;
    return *this;
  }

  constexpr Matrix4& Matrix4::operator-=(const Matrix4& rhs)
  {
    m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02; m03 -= rhs.m03;
    m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13;
    m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23;
    m30 -= rhs.m30; m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33;
    return *this;
  }

  constexpr Matrix4& Matrix4::operator*=(const Matrix4& rhs)
  {
    *this = *this * rhs;
    return *this;
  }

  constexpr Matrix4& Matrix4::operator*=(float scalar)
  {
    m00 *= scalar; m01 *= scalar; m02 *= scalar; m03 *= scalar;
    m10 *= scalar; m11 *= scalar; m12 *= scalar; m13 *= scalar;
    m20 *= scalar; m21 *= scalar; m22 *= scalar; m23 *= scalar;
    m30 *= scalar; m31 *= scalar; m32 *= scalar; m33 *= scalar;
    return *this;
  }

  constexpr bool Matrix4::operator==(const Matrix4& rhs) const
  {
    return m00 == rhs.m00 && m01 == rhs.m01 && m02 == rhs.m02 && m03 == rhs.m03 &&
      m10 == rhs.m10 && m11 == rhs.m11 && m12 == rhs.m12 && m13 == rhs.m13 &&
      m20 == rhs.m20 && m21 == rhs.m21 && m22 == rhs.m22 && m23 == rhs.m23 &&
      m30 == rhs.m30 && m31 == rhs.m31 && m32 == rhs.m32 && m33 == rhs.m33;
  }

  constexpr bool Matrix4::operator!=(const Matrix4& rhs) const
  {
    return !(*this == rhs);
  }

  inline Matrix4 Matrix4::transposed() const
  {
    return Matrix4(
      m00, m10, m20, m30,
      m01, m11, m21, m31,
      m02, m12, m22, m32,
      m03, m13, m23, m33
    );
  }

  inline float Matrix4::determinant() const
  {
    // Laplace (not optimized for performance)
    float det;
    det =
      m00 * (
        m11 * (m22 * m33 - m23 * m32) -
        m12 * (m21 * m33 - m23 * m31) +
        m13 * (m21 * m32 - m22 * m31)
        )
      - m01 * (
        m10 * (m22 * m33 - m23 * m32) -
        m12 * (m20 * m33 - m23 * m30) +
        m13 * (m20 * m32 - m22 * m30)
        )
      + m02 * (
        m10 * (m21 * m33 - m23 * m31) -
        m11 * (m20 * m33 - m23 * m30) +
        m13 * (m20 * m31 - m21 * m30)
        )
      - m03 * (
        m10 * (m21 * m32 - m22 * m31) -
        m11 * (m20 * m32 - m22 * m30) +
        m12 * (m20 * m31 - m21 * m30)
        );

    return det;
  }

  inline Matrix4 Matrix4::inverted() const
  {
    float det = determinant();
    if (det == 0.0f)
      return Matrix4::Zero();

    float invDet = 1.0f / det;

    // Compute cofactors
    Matrix4 cof;

    cof.m00 = m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31);
    cof.m01 = -m01 * (m22 * m33 - m23 * m32) + m02 * (m21 * m33 - m23 * m31) - m03 * (m21 * m32 - m22 * m31);
    cof.m02 = m01 * (m12 * m33 - m13 * m32) - m02 * (m11 * m33 - m13 * m31) + m03 * (m11 * m32 - m12 * m31);
    cof.m03 = -m01 * (m12 * m23 - m13 * m22) + m02 * (m11 * m23 - m13 * m21) - m03 * (m11 * m22 - m12 * m21);

    cof.m10 = -m10 * (m22 * m33 - m23 * m32) + m12 * (m20 * m33 - m23 * m30) - m13 * (m20 * m32 - m22 * m30);
    cof.m11 = m00 * (m22 * m33 - m23 * m32) - m02 * (m20 * m33 - m23 * m30) + m03 * (m20 * m32 - m22 * m30);
    cof.m12 = -m00 * (m12 * m33 - m13 * m32) + m02 * (m10 * m33 - m13 * m30) - m03 * (m10 * m32 - m12 * m30);
    cof.m13 = m00 * (m12 * m23 - m13 * m22) - m02 * (m10 * m23 - m13 * m20) + m03 * (m10 * m22 - m12 * m20);

    cof.m20 = m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) + m13 * (m20 * m31 - m21 * m30);
    cof.m21 = -m00 * (m21 * m33 - m23 * m31) + m01 * (m20 * m33 - m23 * m30) - m03 * (m20 * m31 - m21 * m30);
    cof.m22 = m00 * (m11 * m33 - m13 * m31) - m01 * (m10 * m33 - m13 * m30) + m03 * (m10 * m31 - m11 * m30);
    cof.m23 = -m00 * (m11 * m23 - m13 * m21) + m01 * (m10 * m23 - m13 * m20) - m03 * (m10 * m21 - m11 * m20);

    cof.m30 = -m10 * (m21 * m32 - m22 * m31) + m11 * (m20 * m32 - m22 * m30) - m12 * (m20 * m31 - m21 * m30);
    cof.m31 = m00 * (m21 * m32 - m22 * m31) - m01 * (m20 * m32 - m22 * m30) + m02 * (m20 * m31 - m21 * m30);
    cof.m32 = -m00 * (m11 * m32 - m12 * m31) + m01 * (m10 * m32 - m12 * m30) - m02 * (m10 * m31 - m11 * m30);
    cof.m33 = m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20);

    // Get adjugate
    Matrix4 adj = cof.transposed();

    // Multiply by 1/det
    return adj * invDet;
  }

  inline Matrix4& Matrix4::transpose()
  {
    float tmp;
    tmp = m01; m01 = m10; m10 = tmp;
    tmp = m02; m02 = m20; m20 = tmp;
    tmp = m03; m03 = m30; m30 = tmp;
    tmp = m12; m12 = m21; m21 = tmp;
    tmp = m13; m13 = m31; m31 = tmp;
    tmp = m23; m23 = m32; m32 = tmp;
    return *this;
  }

  inline Matrix4& Matrix4::invert()
  {
    *this = inverted();
    return *this;
  }
}
