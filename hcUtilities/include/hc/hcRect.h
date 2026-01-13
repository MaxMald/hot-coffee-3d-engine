#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcVector2.h"

namespace hc
{
  /**
   * @brief Axis-aligned rectangle template class.
   *
   * Represents a rectangle by its top-left corner (x, y) and its size (width,
   * height). Provides basic rectangle operations such as area calculation,
   * containment, and intersection.
   *
   * @tparam T Numeric type for the rectangle's coordinates and size (e.g., int,
   * float).
   */
  template <typename T>
  struct Rect
  {
    T x{};
    T y{};
    T width{};
    T height{};

    constexpr Rect();
    constexpr Rect(const Rect& other);
    constexpr Rect(const Vector2<T>& position, const Vector2<T>& size);
    constexpr Rect(T xVal, T yVal, T widthVal, T heightVal);

    constexpr bool operator==(const Rect& rhs) const;
    constexpr bool operator!=(const Rect& rhs) const;

    constexpr T area() const;
    constexpr bool contains(T px, T py) const;
    constexpr bool contains(const Vector2<T>& point) const;
  };

  template <typename T>
  constexpr Rect<T>::Rect() = default;

  template<typename T>
  inline constexpr Rect<T>::Rect(const Rect& other) :
    x(other.x), y(other.y), width(other.width), height(other.height)
  {
  }

  template<typename T>
  inline constexpr Rect<T>::Rect(const Vector2<T>& position, const Vector2<T>& size)
  {
    x = position.x;
    y = position.y;
    width = size.x;
    height = size.y;
  }

  template <typename T>
  constexpr Rect<T>::Rect(T xVal, T yVal, T widthVal, T heightVal)
    : x(xVal), y(yVal), width(widthVal), height(heightVal) {
  }

  template <typename T>
  constexpr T Rect<T>::area() const
  {
    return width * height;
  }

  template <typename T>
  constexpr bool Rect<T>::contains(T px, T py) const
  {
    return px >= x && px <= x + width && py >= y && py <= y + height;
  }

  template<typename T>
  inline constexpr bool Rect<T>::contains(const Vector2<T>& point) const
  {
    return contains(point.x, point.y);
  }

  template <typename T>
  constexpr bool Rect<T>::operator==(const Rect& rhs) const
  {
    return x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height;
  }

  template <typename T>
  constexpr bool Rect<T>::operator!=(const Rect& rhs) const
  {
    return !(*this == rhs);
  }

  using RectInt = Rect<Int32>;
  using RectFloat = Rect<float>;
  using RectDouble = Rect<double>;
}
