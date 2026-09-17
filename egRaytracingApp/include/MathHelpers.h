#pragma once

#include <stdint.h>
#include <cmath>
#include <vector>

using Int8 = std::int8_t;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int64 = std::int64_t;
using UInt8 = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
using UInt64 = std::uint64_t;
using UIntPtr = std::uintptr_t;
using Char = char;
using Char16 = char16_t;
using Char32 = char32_t;
using UChar = unsigned char;
using Byte = UInt8;
using SizeT = std::size_t;

/**
 * @file MathHelpers.h
 * @brief Contains mathematical helper classes and functions for 3D graphics.
 *
 * This header defines template classes for 3D and 4D vectors, as well as a Color class.
 * It provides basic arithmetic operations, normalization, dot and cross products, and more.
 *
 * @note The Vector3_T and Vector4 classes are templated to support different numeric types.
 */
template <typename T>
struct Vector3_T
{
  T x{};
  T y{};
  T z{};

  constexpr Vector3_T();
  constexpr Vector3_T(T xVal, T yVal, T zVal);

  // Arithmetic operators
  constexpr Vector3_T operator+(const Vector3_T& rhs) const;
  constexpr Vector3_T operator-(const Vector3_T& rhs) const;
  constexpr Vector3_T operator*(T scalar) const;
  constexpr Vector3_T operator/(T scalar) const;

  // Compound assignment
  constexpr Vector3_T& operator+=(const Vector3_T& rhs);
  constexpr Vector3_T& operator-=(const Vector3_T& rhs);
  constexpr Vector3_T& operator*=(T scalar);
  constexpr Vector3_T& operator/=(T scalar);

  // Equality
  constexpr bool operator==(const Vector3_T& rhs) const;
  constexpr bool operator!=(const Vector3_T& rhs) const;

  T lengthSquared() const;
  T length() const;
  Vector3_T normalized() const;
  T dot(const Vector3_T& rhs) const;
  Vector3_T cross(const Vector3_T<T>& rhs) const;
};

// Definitions (still in header, but outside class)
template <typename T>
constexpr Vector3_T<T>::Vector3_T() = default;

template <typename T>
constexpr Vector3_T<T>::Vector3_T(T xVal, T yVal, T zVal) : x(xVal), y(yVal), z(zVal) {}

template <typename T>
constexpr Vector3_T<T> Vector3_T<T>::operator+(const Vector3_T& rhs) const
{
  return { x + rhs.x, y + rhs.y, z + rhs.z };
}

template <typename T>
constexpr Vector3_T<T> Vector3_T<T>::operator-(const Vector3_T& rhs) const
{
  return { x - rhs.x, y - rhs.y, z - rhs.z };
}

template <typename T>
constexpr Vector3_T<T> Vector3_T<T>::operator*(T scalar) const
{
  return { x * scalar, y * scalar, z * scalar };
}

template <typename T>
constexpr Vector3_T<T> Vector3_T<T>::operator/(T scalar) const
{
  return { x / scalar, y / scalar, z / scalar };
}

template <typename T>
constexpr Vector3_T<T>& Vector3_T<T>::operator+=(const Vector3_T& rhs)
{
  x += rhs.x; y += rhs.y; z += rhs.z;
  return *this;
}

template <typename T>
constexpr Vector3_T<T>& Vector3_T<T>::operator-=(const Vector3_T& rhs)
{
  x -= rhs.x; y -= rhs.y; z -= rhs.z;
  return *this;
}

template <typename T>
constexpr Vector3_T<T>& Vector3_T<T>::operator*=(T scalar)
{
  x *= scalar; y *= scalar; z *= scalar;
  return *this;
}

template <typename T>
constexpr Vector3_T<T>& Vector3_T<T>::operator/=(T scalar)
{
  x /= scalar; y /= scalar; z /= scalar;
  return *this;
}

template <typename T>
constexpr bool Vector3_T<T>::operator==(const Vector3_T& rhs) const
{
  return x == rhs.x && y == rhs.y && z == rhs.z;
}

template <typename T>
constexpr bool Vector3_T<T>::operator!=(const Vector3_T& rhs) const
{
  return !(*this == rhs);
}

template <typename T>
T Vector3_T<T>::lengthSquared() const
{
  return x * x + y * y + z * z;
}

template <typename T>
T Vector3_T<T>::length() const
{
  return static_cast<T>(std::sqrt(static_cast<double>(lengthSquared())));
}

template <typename T>
Vector3_T<T> Vector3_T<T>::normalized() const
{
  T len = length();
  if (len == T{})
    return *this;
  return *this / len;
}

template <typename T>
T Vector3_T<T>::dot(const Vector3_T& rhs) const
{
  return x * rhs.x + y * rhs.y + z * rhs.z;
}

template <typename T>
Vector3_T<T> Vector3_T<T>::cross(const Vector3_T& rhs) const
{
  return {
    y * rhs.z - z * rhs.y,
    z * rhs.x - x * rhs.z,
    x * rhs.y - y * rhs.x
  };
}

// Scalar multiplication from left
template <typename T>
constexpr Vector3_T<T> operator*(T scalar, const Vector3_T<T>& vec)
{
  return vec * scalar;
}

namespace mx
{

}

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
  constexpr Vector4(const Vector3_T<T>& vector3, T wVal);

  // Arithmetic operators
  constexpr Vector4 operator+(const Vector4& rhs) const;
  constexpr Vector4 operator-(const Vector4& rhs) const;
  constexpr Vector4 operator*(const Vector4& rhs) const;
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
  Vector3_T<T> xyz() const;
};

// Definitions (must remain in header for templates)
template <typename T>
constexpr Vector4<T>::Vector4() = default;

template <typename T>
constexpr Vector4<T>::Vector4(T xVal, T yVal, T zVal, T wVal)
  : x(xVal), y(yVal), z(zVal), w(wVal) {
}

template <typename T>
constexpr Vector4<T>::Vector4(const Vector3_T<T>& vector3, T wVal)
  : x(vector3.x), y(vector3.y), z(vector3.z), w(wVal) {
}

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
constexpr Vector4<T> Vector4<T>::operator*(const Vector4& rhs) const
{
  return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w };
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

template <typename T>
Vector3_T<T> Vector4<T>::xyz() const
{
  return { x, y, z };
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

/**
 * @brief Represents a color with red, green, blue, and alpha components.
 *
 * The Color struct provides a convenient way to represent colors in RGBA format.
 * It includes predefined colors and utility functions for color manipulation.
 */
struct Color
{
  static constexpr Color Black();
  static constexpr Color White();
  static constexpr Color Red();
  static constexpr Color Green();
  static constexpr Color Blue();
  static constexpr Color Transparent();

  union
  {
    float m[4];
    struct { float r, g, b, a; };
    Vector4f vec4;
  };

  constexpr Color();
  constexpr Color(float r, float g, float b, float a = 1.0f);
  constexpr Color(const Vector4f& v);

  constexpr bool operator==(const Color& rhs) const;
  constexpr bool operator!=(const Color& rhs) const;

  constexpr Vector4f toVector() const;

  void clamp();
};

constexpr Color Color::Black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
constexpr Color Color::White() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
constexpr Color Color::Red() { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
constexpr Color Color::Green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
constexpr Color Color::Blue() { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
constexpr Color Color::Transparent() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }

constexpr Color::Color()
  : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {
}

constexpr Color::Color(float r, float g, float b, float a)
  : r(r), g(g), b(b), a(a) {
}

constexpr Color::Color(const Vector4f& v)
  : r(v.x), g(v.y), b(v.z), a(v.w) {
}

constexpr bool Color::operator==(const Color& rhs) const
{
  return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
}

constexpr bool Color::operator!=(const Color& rhs) const
{
  return !(*this == rhs);
}

constexpr Vector4f Color::toVector() const
{
  return Vector4f(r, g, b, a);
}

inline void Color::clamp()
{
  r = (r < 0.0f) ? 0.0f : (r > 1.0f) ? 1.0f : r;
  g = (g < 0.0f) ? 0.0f : (g > 1.0f) ? 1.0f : g;
  b = (b < 0.0f) ? 0.0f : (b > 1.0f) ? 1.0f : b;
  a = (a < 0.0f) ? 0.0f : (a > 1.0f) ? 1.0f : a;
}

/**
 * @brief Represents a sphere in 3D space with associated properties.
 *
 * This template class defines a sphere with a center, radius, and material coefficients.
 * It can be used in ray tracing and other 3D graphics applications.
 *
 * @tparam Real Numeric type for the sphere's properties (e.g., float, double).
 */
template<typename Real>
class Sphere_T
{
public:
  Sphere_T() {};
  Sphere_T(
    const Vector3_T<Real>& center,
    Real radius,
    const Color& color,
    Real kA,
    Real kD,
    Real kS
  )
    : center(center),
    radius(radius),
    color(color),
    coeffs(kA, kD, kS) // ambient diffuse specular coefficients
  {
  }

public:
  Color color;
  Vector3_T<Real> center;
  Vector3_T<Real> coeffs;
  Real radius;
};

template<typename Real>
class Plane_T
{
public:
  Plane_T() {};
  Plane_T(
    const Vector3_T<Real>& point,
    const Vector3_T<Real>& normal,
    const Color& color,
    Real kA,
    Real kD,
    Real kS
  )
    : point(point),
    normal(normal.normalized()),
    color(color),
    coeffs(kA, kD, kS) // ambient diffuse specular coefficients
  {
  }

public:
  Vector3_T<Real> point;
  Vector3_T<Real> normal;
  Color color;
  Vector3_T<Real> coeffs;
};

/**
 * @brief Represents a ray in 3D space with an origin and direction.
 *
 * This template class defines a ray with an origin point and a direction vector.
 * It provides a method to compute points along the ray based on a parameter t.
 *
 * @tparam Real Numeric type for the ray's properties (e.g., float, double).
 */
template<typename Real>
class Ray_T
{
public:
  Ray_T() = default;
  Ray_T(
    const Vector3_T<Real>& origin,
    const Vector3_T<Real>& direction
  )
    : origin(origin), direction(direction.normalized())
  {
  }

  Vector3_T<Real> where(Real t) const
  {
    return origin + direction * t;
  }

public:
  Vector3_T<Real> origin;
  Vector3_T<Real> direction;
};

#define REAL_TYPE float
using Vector3 = Vector3_T<REAL_TYPE>;
using Ray = Ray_T<REAL_TYPE>;
using Sphere = Sphere_T<REAL_TYPE>;
using Plane = Plane_T<REAL_TYPE>;

struct HitInfo
{
  REAL_TYPE distance;
  Vector3 position;
  Vector3 normal;
  Sphere* pSphere = nullptr;
  Plane* pPlane = nullptr;
};

template <typename T>
using Vector = std::vector<T>;