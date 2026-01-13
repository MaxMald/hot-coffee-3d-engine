#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include <cmath>
#include <type_traits>

namespace hc
{
  /**
   * @brief 3D vector template class.
   *
   * Represents a mathematical vector in 3D space with components of type T.
   *
   * @tparam T Numeric type for the vector components (e.g., int, float, double).
   *
   * @note Common type aliases: Vector3i (int), Vector3f (float), Vector3d
   * (double).
   */
  template <typename T>
  struct Vector3
  {
    T x{};
    T y{};
    T z{};

    constexpr Vector3();
    constexpr Vector3(T xVal, T yVal, T zVal);
    constexpr Vector3(const Vector3& other);

    constexpr Vector3& operator=(const Vector3& other);

    // Arithmetic operators
    constexpr Vector3 operator+(const Vector3& rhs) const;
    constexpr Vector3 operator-(const Vector3& rhs) const;
    constexpr Vector3 operator*(T scalar) const;
    constexpr Vector3 operator/(T scalar) const;

    // Compound assignment
    constexpr Vector3& operator+=(const Vector3& rhs);
    constexpr Vector3& operator-=(const Vector3& rhs);
    constexpr Vector3& operator*=(T scalar);
    constexpr Vector3& operator/=(T scalar);

    // Equality
    constexpr bool operator==(const Vector3& rhs) const;
    constexpr bool operator!=(const Vector3& rhs) const;

    T lengthSquared() const;
    T length() const;
    Vector3 normalized() const;
    T dot(const Vector3& rhs) const;
    Vector3 cross(const Vector3& rhs) const;
  };

  // Definitions (still in header, but outside class)
  template <typename T>
  constexpr Vector3<T>::Vector3() = default;

  template <typename T>
  constexpr Vector3<T>::Vector3(T xVal, T yVal, T zVal) : x(xVal), y(yVal), z(zVal) {}

  template <typename T>
  constexpr Vector3<T>::Vector3(const Vector3& other) = default;

  template <typename T>
  constexpr Vector3<T>& Vector3<T>::operator=(const Vector3& other) = default;

  template <typename T>
  constexpr Vector3<T> Vector3<T>::operator+(const Vector3& rhs) const
  {
    return { x + rhs.x, y + rhs.y, z + rhs.z };
  }

  template <typename T>
  constexpr Vector3<T> Vector3<T>::operator-(const Vector3& rhs) const
  {
    return { x - rhs.x, y - rhs.y, z - rhs.z };
  }

  template <typename T>
  constexpr Vector3<T> Vector3<T>::operator*(T scalar) const
  {
    return { x * scalar, y * scalar, z * scalar };
  }

  template <typename T>
  constexpr Vector3<T> Vector3<T>::operator/(T scalar) const
  {
    return { x / scalar, y / scalar, z / scalar };
  }

  template <typename T>
  constexpr Vector3<T>& Vector3<T>::operator+=(const Vector3& rhs)
  {
    x += rhs.x; y += rhs.y; z += rhs.z;
    return *this;
  }

  template <typename T>
  constexpr Vector3<T>& Vector3<T>::operator-=(const Vector3& rhs)
  {
    x -= rhs.x; y -= rhs.y; z -= rhs.z;
    return *this;
  }

  template <typename T>
  constexpr Vector3<T>& Vector3<T>::operator*=(T scalar)
  {
    x *= scalar; y *= scalar; z *= scalar;
    return *this;
  }

  template <typename T>
  constexpr Vector3<T>& Vector3<T>::operator/=(T scalar)
  {
    x /= scalar; y /= scalar; z /= scalar;
    return *this;
  }

  template <typename T>
  constexpr bool Vector3<T>::operator==(const Vector3& rhs) const
  {
    return x == rhs.x && y == rhs.y && z == rhs.z;
  }

  template <typename T>
  constexpr bool Vector3<T>::operator!=(const Vector3& rhs) const
  {
    return !(*this == rhs);
  }

  template <typename T>
  T Vector3<T>::lengthSquared() const
  {
    return x * x + y * y + z * z;
  }

  template <typename T>
  T Vector3<T>::length() const
  {
    return static_cast<T>(std::sqrt(static_cast<double>(lengthSquared())));
  }

  template <typename T>
  Vector3<T> Vector3<T>::normalized() const
  {
    T len = length();
    if (len == T{})
      return *this;
    return *this / len;
  }

  template <typename T>
  T Vector3<T>::dot(const Vector3& rhs) const
  {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }

  template <typename T>
  Vector3<T> Vector3<T>::cross(const Vector3& rhs) const
  {
    return {
      y * rhs.z - z * rhs.y,
      z * rhs.x - x * rhs.z,
      x * rhs.y - y * rhs.x
    };
  }

  // Scalar multiplication from left
  template <typename T>
  constexpr Vector3<T> operator*(T scalar, const Vector3<T>& vec)
  {
    return vec * scalar;
  }

  using Vector3i = Vector3<Int32>;
  using Vector3f = Vector3<float>;
  using Vector3d = Vector3<double>;
}
