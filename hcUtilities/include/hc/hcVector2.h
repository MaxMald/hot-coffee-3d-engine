#pragma once

#include "hc/hcUtilitiesPrerequisites.h"

namespace hc
{
  /**
   * @brief 2D vector template class.
   *
   * Represents a mathematical vector in 2D space with components of type T.
   *
   * @tparam T Numeric type for the vector components (e.g., int, float, double).
   *
   * @note Common type aliases: Vector2i (int), Vector2f (float), Vector2d
   * (double).
   */
  template <typename T>
  struct Vector2
  {
    T x{};
    T y{};

    constexpr Vector2();
    constexpr Vector2(T xVal, T yVal);
    constexpr Vector2(const Vector2& other);

    constexpr Vector2& operator=(const Vector2& other);

    // Arithmetic operators
    constexpr Vector2 operator+(const Vector2& rhs) const;
    constexpr Vector2 operator-(const Vector2& rhs) const;
    constexpr Vector2 operator*(T scalar) const;
    constexpr Vector2 operator/(T scalar) const;

    // Compound assignment
    constexpr Vector2& operator+=(const Vector2& rhs);
    constexpr Vector2& operator-=(const Vector2& rhs);
    constexpr Vector2& operator*=(T scalar);
    constexpr Vector2& operator/=(T scalar);

    // Equality
    constexpr bool operator==(const Vector2& rhs) const;
    constexpr bool operator!=(const Vector2& rhs) const;

    T lengthSquared() const;
    T length() const;
    Vector2 normalized() const;
    T dot(const Vector2& rhs) const;
  };

  // Definitions (still in header, but outside class)
  template <typename T>
  constexpr Vector2<T>::Vector2() = default;

  template <typename T>
  constexpr Vector2<T>::Vector2(T xVal, T yVal) : x(xVal), y(yVal) {}

  template <typename T>
  constexpr Vector2<T>::Vector2(const Vector2& other) = default;

  template <typename T>
  constexpr Vector2<T>& Vector2<T>::operator=(const Vector2& other) = default;

  template <typename T>
  constexpr Vector2<T> Vector2<T>::operator+(const Vector2& rhs) const
  {
    return { x + rhs.x, y + rhs.y };
  }

  template <typename T>
  constexpr Vector2<T> Vector2<T>::operator-(const Vector2& rhs) const
  {
    return { x - rhs.x, y - rhs.y };
  }

  template <typename T>
  constexpr Vector2<T> Vector2<T>::operator*(T scalar) const
  {
    return { x * scalar, y * scalar };
  }

  template <typename T>
  constexpr Vector2<T> Vector2<T>::operator/(T scalar) const
  {
    return { x / scalar, y / scalar };
  }

  template <typename T>
  constexpr Vector2<T>& Vector2<T>::operator+=(const Vector2& rhs)
  {
    x += rhs.x; y += rhs.y;
    return *this;
  }

  template <typename T>
  constexpr Vector2<T>& Vector2<T>::operator-=(const Vector2& rhs)
  {
    x -= rhs.x; y -= rhs.y;
    return *this;
  }

  template <typename T>
  constexpr Vector2<T>& Vector2<T>::operator*=(T scalar)
  {
    x *= scalar; y *= scalar;
    return *this;
  }

  template <typename T>
  constexpr Vector2<T>& Vector2<T>::operator/=(T scalar)
  {
    x /= scalar; y /= scalar;
    return *this;
  }

  template <typename T>
  constexpr bool Vector2<T>::operator==(const Vector2& rhs) const
  {
    return x == rhs.x && y == rhs.y;
  }

  template <typename T>
  constexpr bool Vector2<T>::operator!=(const Vector2& rhs) const
  {
    return !(*this == rhs);
  }

  template <typename T>
  T Vector2<T>::lengthSquared() const
  {
    return x * x + y * y;
  }

  template <typename T>
  T Vector2<T>::length() const
  {
    return static_cast<T>(std::sqrt(static_cast<double>(lengthSquared())));
  }

  template <typename T>
  Vector2<T> Vector2<T>::normalized() const
  {
    T len = length();
    if (len == T{})
      return *this;
    return *this / len;
  }

  template <typename T>
  T Vector2<T>::dot(const Vector2& rhs) const { return x * rhs.x + y * rhs.y; }

  // Scalar multiplication from left
  template <typename T>
  constexpr Vector2<T> operator*(T scalar, const Vector2<T>& vec)
  {
    return vec * scalar;
  }

  using Vector2i = Vector2<Int32>;
  using Vector2u = Vector2<UInt32>;
  using Vector2f = Vector2<float>;
  using Vector2d = Vector2<double>;
}
