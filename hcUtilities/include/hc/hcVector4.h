#pragma once

#include "hc/hcUtilitiesPrerequisites.h"

namespace hc
{
  /**
   * @brief 4D vector template class.
   *
   * Represents a mathematical vector in 4D space with components of type T.
   *
   * @tparam T Numeric type for the vector components (e.g., int, float, double).
   *
   * @note Common type aliases: Vector4i (int), Vector4f (float), Vector4d (double).
   */
  template <typename T>
  struct Vector4
  {
    T x{};
    T y{};
    T z{};
    T w{};

    constexpr Vector4();
    constexpr Vector4(T xVal, T yVal, T zVal, T wVal);
    constexpr Vector4(const Vector4& other);

    constexpr Vector4& operator=(const Vector4& other);

    // Arithmetic operators
    constexpr Vector4 operator+(const Vector4& rhs) const;
    constexpr Vector4 operator-(const Vector4& rhs) const;
    constexpr Vector4 operator*(T scalar) const;
    constexpr Vector4 operator/(T scalar) const;

    // Compound assignment
    constexpr Vector4& operator+=(const Vector4& rhs);
    constexpr Vector4& operator-=(const Vector4& rhs);
    constexpr Vector4& operator*=(T scalar);
    constexpr Vector4& operator/=(T scalar);

    // Equality
    constexpr bool operator==(const Vector4& rhs) const;
    constexpr bool operator!=(const Vector4& rhs) const;

    T lengthSquared() const;
    T length() const;
    Vector4 normalized() const;
    T dot(const Vector4& rhs) const;
  };

  // Definitions (must remain in header for templates)
  template <typename T>
  constexpr Vector4<T>::Vector4() = default;

  template <typename T>
  constexpr Vector4<T>::Vector4(T xVal, T yVal, T zVal, T wVal)
    : x(xVal), y(yVal), z(zVal), w(wVal) {}

  template <typename T>
  constexpr Vector4<T>::Vector4(const Vector4& other) = default;

  template <typename T>
  constexpr Vector4<T>& Vector4<T>::operator=(const Vector4& other) = default;

  template <typename T>
  constexpr Vector4<T> Vector4<T>::operator+(const Vector4& rhs) const
  {
    return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
  }

  template <typename T>
  constexpr Vector4<T> Vector4<T>::operator-(const Vector4& rhs) const
  {
    return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
  }

  template <typename T>
  constexpr Vector4<T> Vector4<T>::operator*(T scalar) const
  {
    return { x * scalar, y * scalar, z * scalar, w * scalar };
  }

  template <typename T>
  constexpr Vector4<T> Vector4<T>::operator/(T scalar) const
  {
    return { x / scalar, y / scalar, z / scalar, w / scalar };
  }

  template <typename T>
  constexpr Vector4<T>& Vector4<T>::operator+=(const Vector4& rhs)
  {
    x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
    return *this;
  }

  template <typename T>
  constexpr Vector4<T>& Vector4<T>::operator-=(const Vector4& rhs)
  {
    x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
    return *this;
  }

  template <typename T>
  constexpr Vector4<T>& Vector4<T>::operator*=(T scalar)
  {
    x *= scalar; y *= scalar; z *= scalar; w *= scalar;
    return *this;
  }

  template <typename T>
  constexpr Vector4<T>& Vector4<T>::operator/=(T scalar)
  {
    x /= scalar; y /= scalar; z /= scalar; w /= scalar;
    return *this;
  }

  template <typename T>
  constexpr bool Vector4<T>::operator==(const Vector4& rhs) const
  {
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
  }

  template <typename T>
  constexpr bool Vector4<T>::operator!=(const Vector4& rhs) const
  {
    return !(*this == rhs);
  }

  template <typename T>
  T Vector4<T>::lengthSquared() const
  {
    return x * x + y * y + z * z + w * w;
  }

  template <typename T>
  T Vector4<T>::length() const
  {
    return static_cast<T>(std::sqrt(static_cast<double>(lengthSquared())));
  }

  template <typename T>
  Vector4<T> Vector4<T>::normalized() const
  {
    T len = length();
    if (len == T{}) 
      return *this;

    return *this / len;
  }

  template <typename T>
  T Vector4<T>::dot(const Vector4& rhs) const
  {
    return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
  }

  // Scalar multiplication from left
  template <typename T>
  constexpr Vector4<T> operator*(T scalar, const Vector4<T>& vec)
  {
    return vec * scalar;
  }

  using Vector4i = Vector4<int>;
  using Vector4f = Vector4<float>;
  using Vector4d = Vector4<double>;
}
