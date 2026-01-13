#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcVector3.h"

namespace hc
{
  /**
   * @brief 3x3 matrix class for linear algebra operations.
   *
   * Represents a 3x3 matrix with float components.
   *
   * Members can be accessed via m[3][3], m00/m01/m02/m10/m11/m12/m20/m21/m22, or
   * as three Vector3f rows.
   */
  struct Matrix3
  {
    // Common matrices
    static constexpr Matrix3 Identity();
    static constexpr Matrix3 Zero();

    union
    {
      float m[3][3];

      struct
      {
        float m00, m01, m02,
              m10, m11, m12,
              m20, m21, m22;
      };

      Vector3f mRow[3];
    };

    constexpr Matrix3();
    constexpr Matrix3(
      float m00, float m01, float m02,
      float m10, float m11, float m12,
      float m20, float m21, float m22
    );
    constexpr Matrix3(const Matrix3& other);

    constexpr Matrix3& operator=(const Matrix3& other);

    // Arithmetic operators
    constexpr Matrix3 operator+(const Matrix3& rhs) const;
    constexpr Matrix3 operator-(const Matrix3& rhs) const;
    constexpr Matrix3 operator*(const Matrix3& rhs) const;
    constexpr Vector3f operator*(const Vector3f& vec) const;
    constexpr Matrix3 operator*(float scalar) const;

    // Compound assignment
    constexpr Matrix3& operator+=(const Matrix3& rhs);
    constexpr Matrix3& operator-=(const Matrix3& rhs);
    constexpr Matrix3& operator*=(const Matrix3& rhs);
    constexpr Matrix3& operator*=(float scalar);

    // Equality
    constexpr bool operator==(const Matrix3& rhs) const;
    constexpr bool operator!=(const Matrix3& rhs) const;

    inline Matrix3 transposed() const;
    inline Matrix3 inverted() const;
    inline float determinant() const;
    inline Matrix3& transpose();
    inline Matrix3& invert();
  };

  // Definitions (must remain in header for constexpr)
  constexpr Matrix3::Matrix3()
    : m00(0.0f), m01(0.0f), m02(0.0f),
      m10(0.0f), m11(0.0f), m12(0.0f),
      m20(0.0f), m21(0.0f), m22(0.0f)
  {}

  constexpr Matrix3::Matrix3(
    float m00, float m01, float m02,
    float m10, float m11, float m12,
    float m20, float m21, float m22
  )
    : m00(m00), m01(m01), m02(m02),
      m10(m10), m11(m11), m12(m12),
      m20(m20), m21(m21), m22(m22)
  {}

  constexpr Matrix3::Matrix3(const Matrix3& other)
    : m00(other.m00), m01(other.m01), m02(other.m02),
      m10(other.m10), m11(other.m11), m12(other.m12),
      m20(other.m20), m21(other.m21), m22(other.m22)
  {}

  constexpr Matrix3& Matrix3::operator=(const Matrix3& other)
  {
    m00 = other.m00; m01 = other.m01; m02 = other.m02;
    m10 = other.m10; m11 = other.m11; m12 = other.m12;
    m20 = other.m20; m21 = other.m21; m22 = other.m22;
    return *this;
  }

  constexpr Matrix3 Matrix3::operator+(const Matrix3& rhs) const
  {
    return Matrix3(
      m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02,
      m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12,
      m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22
    );
  }

  constexpr Matrix3 Matrix3::operator-(const Matrix3& rhs) const
  {
    return Matrix3(
      m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02,
      m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12,
      m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22
    );
  }

  constexpr Matrix3 Matrix3::operator*(const Matrix3& rhs) const
  {
    return Matrix3(
      m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20,
      m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21,
      m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22,

      m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20,
      m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21,
      m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22,

      m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20,
      m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21,
      m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22
    );
  }

  constexpr Vector3f Matrix3::operator*(const Vector3f& vec) const
  {
    return Vector3f(
      m00 * vec.x + m01 * vec.y + m02 * vec.z,
      m10 * vec.x + m11 * vec.y + m12 * vec.z,
      m20 * vec.x + m21 * vec.y + m22 * vec.z
    );
  }

  constexpr Matrix3 Matrix3::operator*(float scalar) const
  {
    return Matrix3(
      m00 * scalar, m01 * scalar, m02 * scalar,
      m10 * scalar, m11 * scalar, m12 * scalar,
      m20 * scalar, m21 * scalar, m22 * scalar
    );
  }

  constexpr Matrix3& Matrix3::operator+=(const Matrix3& rhs)
  {
    m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02;
    m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12;
    m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22;
    return *this;
  }

  constexpr Matrix3& Matrix3::operator-=(const Matrix3& rhs)
  {
    m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02;
    m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12;
    m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22;
    return *this;
  }

  constexpr Matrix3& Matrix3::operator*=(const Matrix3& rhs)
  {
    *this = *this * rhs;
    return *this;
  }

  constexpr Matrix3& Matrix3::operator*=(float scalar)
  {
    m00 *= scalar; m01 *= scalar; m02 *= scalar;
    m10 *= scalar; m11 *= scalar; m12 *= scalar;
    m20 *= scalar; m21 *= scalar; m22 *= scalar;
    return *this;
  }

  constexpr bool Matrix3::operator==(const Matrix3& rhs) const
  {
    return m00 == rhs.m00 && m01 == rhs.m01 && m02 == rhs.m02 &&
           m10 == rhs.m10 && m11 == rhs.m11 && m12 == rhs.m12 &&
           m20 == rhs.m20 && m21 == rhs.m21 && m22 == rhs.m22;
  }

  constexpr bool Matrix3::operator!=(const Matrix3& rhs) const
  {
    return !(*this == rhs);
  }

  inline Matrix3 Matrix3::transposed() const
  {
    return Matrix3(
      m00, m10, m20,
      m01, m11, m21,
      m02, m12, m22
    );
  }

  inline float Matrix3::determinant() const
  {
    return
      m00 * (m11 * m22 - m12 * m21) -
      m01 * (m10 * m22 - m12 * m20) +
      m02 * (m10 * m21 - m11 * m20);
  }

  inline Matrix3 Matrix3::inverted() const
  {
    float det = determinant();
    if (det == 0.0f)
      return Matrix3::Zero();

    float invDet = 1.0f / det;

    return Matrix3(
      (m11 * m22 - m12 * m21) * invDet,
      (m02 * m21 - m01 * m22) * invDet,
      (m01 * m12 - m02 * m11) * invDet,

      (m12 * m20 - m10 * m22) * invDet,
      (m00 * m22 - m02 * m20) * invDet,
      (m02 * m10 - m00 * m12) * invDet,

      (m10 * m21 - m11 * m20) * invDet,
      (m01 * m20 - m00 * m21) * invDet,
      (m00 * m11 - m01 * m10) * invDet
    );
  }

  inline Matrix3& Matrix3::transpose()
  {
    float tmp;
    tmp = m01; m01 = m10; m10 = tmp;
    tmp = m02; m02 = m20; m20 = tmp;
    tmp = m12; m12 = m21; m21 = tmp;
    return *this;
  }

  inline Matrix3& Matrix3::invert()
  {
    *this = inverted();
    return *this;
  }

  constexpr Matrix3 Matrix3::Identity()
  {
    return Matrix3(
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f
    );
  }

  constexpr Matrix3 Matrix3::Zero()
  {
    return Matrix3(
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f
    );
  }
}
