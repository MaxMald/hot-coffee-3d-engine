#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcVector2.h"

namespace hc
{
  /**
   * @brief 2x2 matrix class for linear algebra operations.
   *
   * Represents a 2x2 matrix with float components.
   *
   * Members can be accessed via m[2][2], m00/m01/m10/m11, or as two Vector2f rows.
   */
  struct Matrix2
  {
    static constexpr Matrix2 Identity();
    static constexpr Matrix2 Zero();

    union
    {
      float m[2][2];

      struct
      {
        float m00, m01,
          m10, m11;
      };

      Vector2f mRow[2];
    };

    constexpr Matrix2();
    constexpr Matrix2(float m00, float m01, float m10, float m11);
    constexpr Matrix2(const Matrix2& other);

    constexpr Matrix2& operator=(const Matrix2& other);

    // Arithmetic operators
    constexpr Matrix2 operator+(const Matrix2& rhs) const;
    constexpr Matrix2 operator-(const Matrix2& rhs) const;
    constexpr Matrix2 operator*(const Matrix2& rhs) const;
    constexpr Vector2f operator*(const Vector2f& vec) const;
    constexpr Matrix2 operator*(float scalar) const;

    // Compound assignment
    constexpr Matrix2& operator+=(const Matrix2& rhs);
    constexpr Matrix2& operator-=(const Matrix2& rhs);
    constexpr Matrix2& operator*=(const Matrix2& rhs);
    constexpr Matrix2& operator*=(float scalar);

    // Equality
    constexpr bool operator==(const Matrix2& rhs) const;
    constexpr bool operator!=(const Matrix2& rhs) const;

    inline Matrix2 transposed() const;
    inline Matrix2 inverted() const;
    inline float determinant() const;
    inline Matrix2& transpose();
    inline Matrix2& invert();
  };

  constexpr Matrix2 Matrix2::Identity()
  {
    return Matrix2(1.0f, 0.0f, 0.0f, 1.0f);
  }

  constexpr Matrix2 Matrix2::Zero()
  {
    return Matrix2(0.0f, 0.0f, 0.0f, 0.0f);
  }

  constexpr Matrix2::Matrix2() :
    m00(0.0f), m01(0.0f),
    m10(0.0f), m11(0.0f)
  {
  }

  constexpr Matrix2::Matrix2(float m00, float m01, float m10, float m11) :
    m00(m00), m01(m01),
    m10(m10), m11(m11)
  {
  }

  constexpr Matrix2::Matrix2(const Matrix2& other) :
    m00(other.m00), m01(other.m01),
    m10(other.m10), m11(other.m11)
  {
  }

  constexpr Matrix2& Matrix2::operator=(const Matrix2& other)
  {
    m00 = other.m00; m01 = other.m01;
    m10 = other.m10; m11 = other.m11;
    return *this;
  }

  constexpr Matrix2 Matrix2::operator+(const Matrix2& rhs) const
  {
    return Matrix2(
      m00 + rhs.m00, m01 + rhs.m01,
      m10 + rhs.m10, m11 + rhs.m11
    );
  }

  constexpr Matrix2 Matrix2::operator-(const Matrix2& rhs) const
  {
    return Matrix2(
      m00 - rhs.m00, m01 - rhs.m01,
      m10 - rhs.m10, m11 - rhs.m11
    );
  }

  constexpr Matrix2 Matrix2::operator*(const Matrix2& rhs) const
  {
    return Matrix2(
      m00 * rhs.m00 + m01 * rhs.m10, m00 * rhs.m01 + m01 * rhs.m11,
      m10 * rhs.m00 + m11 * rhs.m10, m10 * rhs.m01 + m11 * rhs.m11
    );
  }

  constexpr Vector2f Matrix2::operator*(const Vector2f& vec) const
  {
    return Vector2f(
      m00 * vec.x + m01 * vec.y,
      m10 * vec.x + m11 * vec.y
    );
  }

  constexpr Matrix2 Matrix2::operator*(float scalar) const
  {
    return Matrix2(
      m00 * scalar, m01 * scalar,
      m10 * scalar, m11 * scalar
    );
  }

  constexpr Matrix2& Matrix2::operator+=(const Matrix2& rhs)
  {
    m00 += rhs.m00; m01 += rhs.m01;
    m10 += rhs.m10; m11 += rhs.m11;
    return *this;
  }

  constexpr Matrix2& Matrix2::operator-=(const Matrix2& rhs)
  {
    m00 -= rhs.m00; m01 -= rhs.m01;
    m10 -= rhs.m10; m11 -= rhs.m11;
    return *this;
  }

  constexpr Matrix2& Matrix2::operator*=(const Matrix2& rhs)
  {
    *this = *this * rhs;
    return *this;
  }

  constexpr Matrix2& Matrix2::operator*=(float scalar)
  {
    m00 *= scalar; m01 *= scalar;
    m10 *= scalar; m11 *= scalar;
    return *this;
  }

  constexpr bool Matrix2::operator==(const Matrix2& rhs) const
  {
    return m00 == rhs.m00 && m01 == rhs.m01 &&
      m10 == rhs.m10 && m11 == rhs.m11;
  }

  constexpr bool Matrix2::operator!=(const Matrix2& rhs) const
  {
    return !(*this == rhs);
  }

  inline Matrix2 Matrix2::transposed() const
  {
    return Matrix2(
      m00, m10,
      m01, m11
    );
  }

  inline float Matrix2::determinant() const
  {
    return m00 * m11 - m01 * m10;
  }

  inline Matrix2 Matrix2::inverted() const
  {
    float det = determinant();
    if (det == 0.0f)
      return Matrix2::Zero();

    float invDet = 1.0f / det;

    return Matrix2(
      m11 * invDet, -m01 * invDet,
      -m10 * invDet, m00 * invDet
    );
  }

  inline Matrix2& Matrix2::transpose()
  {
    float tmp = m01;
    m01 = m10;
    m10 = tmp;
    return *this;
  }

  inline Matrix2& Matrix2::invert()
  {
    *this = inverted();
    return *this;
  }
}
