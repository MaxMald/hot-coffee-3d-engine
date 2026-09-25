#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcVector4.h"
#include "hc/utilities/hcMath.h"

namespace hc
{
  struct Color
  {
    static constexpr Color Black();
    static constexpr Color White();
    static constexpr Color Red();
    static constexpr Color Green();
    static constexpr Color Blue();
    static constexpr Color Gold();
    static constexpr Color Transparent();
    inline static Color CreateHSV(UInt8 h, UInt8 s, UInt8 v);
    inline static Color CreateHSV(float h, float s, float v);
    inline static Color RandomHSV(float brightness);
    inline static Color RandomHSV(float brightness, float saturation);
    inline static Color Lerp(const Color& a, const Color& b, float t);

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

    constexpr Color operator+(const Color& rhs) const;
    constexpr Color operator-(const Color& rhs) const;
    constexpr Color operator*(const Color& rhs) const;
    constexpr Color operator/(const Color& rhs) const;
    constexpr Color operator*(float scalar) const;
    constexpr Color operator/(float scalar) const;

    constexpr Color& operator+=(const Color& rhs);
    constexpr Color& operator-=(const Color& rhs);
    constexpr Color& operator*=(const Color& rhs);
    constexpr Color& operator/=(const Color& rhs);
    constexpr Color& operator*=(float scalar);
    constexpr Color& operator/=(float scalar);

    constexpr Vector4f toVector() const;

    inline Color& setRGBA(UInt8 r, UInt8 g, UInt8 b, UInt8 a = static_cast<UInt8>(0));
    inline Color& setHSV(UInt8 h, UInt8 s, UInt8 v);
    inline Color& setHSV(float h, float s, float v);
    inline void getHSV(UInt8& h, UInt8& s, UInt8& v);
    inline void getHSV(float& h, float& s, float& v);
    inline void getRGB(UInt8& r, UInt8& g, UInt8& b);
    inline void getRGBA(UInt8& r, UInt8& g, UInt8& b, UInt8& a);
    inline void clamp();
  };

  constexpr Color Color::Black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
  constexpr Color Color::White() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
  constexpr Color Color::Red() { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
  constexpr Color Color::Green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
  constexpr Color Color::Blue() { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
  constexpr Color Color::Gold() { return Color(1.0f, 0.72f, 0.315f, 1.0f); }
  constexpr Color Color::Transparent() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }

  inline Color Color::CreateHSV(UInt8 h, UInt8 s, UInt8 v)
  {
    Color color;
    color.setHSV(h, s, v);
    return color;
  }

  inline Color Color::CreateHSV(float h, float s, float v)
  {
    UInt8 hue = static_cast<UInt8>(h * 255.0f);
    UInt8 sat = static_cast<UInt8>(s * 255.0f);
    UInt8 val = static_cast<UInt8>(v * 255.0f);
    return CreateHSV(hue, sat, val);
  }

  inline Color Color::RandomHSV(float brightness)
  {
    UInt8 h = static_cast<UInt8>(rand() % 256);
    UInt8 s = static_cast<UInt8>(rand() % 256);
    return Color::CreateHSV(h, s, static_cast<UInt8>(brightness * 255));
  }

  inline Color Color::RandomHSV(float brightness, float saturation)
  {
    UInt8 h = static_cast<UInt8>(rand() % 256);
    return Color::CreateHSV(h, static_cast<UInt8>(saturation * 255), static_cast<UInt8>(brightness * 255));
  }

  inline Color Color::Lerp(const Color& a, const Color& b, float t)
  {
    return a * (1.0f - t) + b * t;
  }

  constexpr Color::Color()
    : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {
  }

  constexpr Color::Color(float _r, float _g, float _b, float _a)
    : r(_r), g(_g), b(_b), a(_a) {
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

  constexpr Color Color::operator+(const Color& rhs) const
  {
    return Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
  }

  constexpr Color Color::operator-(const Color& rhs) const
  {
    return Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
  }

  constexpr Color Color::operator*(const Color& rhs) const
  {
    return Color(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a);
  }

  constexpr Color Color::operator/(const Color& rhs) const
  {
    return Color(r / rhs.r, g / rhs.g, b / rhs.b, a / rhs.a);
  }

  constexpr Color Color::operator*(float scalar) const
  {
    return Color(r * scalar, g * scalar, b * scalar, a * scalar);
  }

  constexpr Color Color::operator/(float scalar) const
  {
    return Color(r / scalar, g / scalar, b / scalar, a / scalar);
  }

  constexpr Color& Color::operator+=(const Color& rhs)
  {
    r += rhs.r; g += rhs.g; b += rhs.b; a += rhs.a;
    return *this;
  }

  constexpr Color& Color::operator-=(const Color& rhs)
  {
    r -= rhs.r; g -= rhs.g; b -= rhs.b; a -= rhs.a;
    return *this;
  }

  constexpr Color& Color::operator*=(const Color& rhs)
  {
    r *= rhs.r; g *= rhs.g; b *= rhs.b; a *= rhs.a;
    return *this;
  }

  constexpr Color& Color::operator/=(const Color& rhs)
  {
    r /= rhs.r; g /= rhs.g; b /= rhs.b; a /= rhs.a;
    return *this;
  }

  constexpr Color& Color::operator*=(float scalar)
  {
    r *= scalar; g *= scalar; b *= scalar; a *= scalar;
    return *this;
  }

  constexpr Color& Color::operator/=(float scalar)
  {
    r /= scalar; g /= scalar; b /= scalar; a /= scalar;
    return *this;
  }

  constexpr Vector4f Color::toVector() const
  {
    return Vector4f(r, g, b, a);
  }

  inline Color& Color::setRGBA(UInt8 _r, UInt8 _g, UInt8 _b, UInt8 _a)
  {
    this->r = _r / 255.0f;
    this->g = _g / 255.0f;
    this->b = _b / 255.0f;
    this->a = _a / 255.0f;
    return *this;
  }

  inline Color& Color::setHSV(UInt8 h, UInt8 s, UInt8 v)
  {
    float brightness = v * 1.4f / 255.0f;
    brightness *= 0.7f / 0.01f + Math::Sqrt(brightness);
    brightness = Math::Min(Math::Max(brightness, 0.0f), 1.0f);

    const Vector3f hue = (h < 86) ?
      Vector3f((85 - h) / 85.0f, (h - 0) / 85.0f, 0.0f) : (h < 171) ?
      Vector3f(0, (170 - h) / 85.0f, (h - 85) / 85.0f) :
      Vector3f((h - 170) / 85.0f, 0.0f, (255 - h) / 84.0f);

    const Vector3f colorVec = (hue + s / 255.0f * (Vector3f(1.0f, 1.0f, 1.0f) - hue)) * brightness;

    r = colorVec.x;
    g = colorVec.y;
    b = colorVec.z;
    return *this;
  }

  inline Color& Color::setHSV(float h, float s, float v)
  {
    UInt8 hue = static_cast<UInt8>(h * 255.0f);
    UInt8 sat = static_cast<UInt8>(s * 255.0f);
    UInt8 val = static_cast<UInt8>(v * 255.0f);
    return setHSV(hue, sat, val);
  }

  inline void Color::getHSV(UInt8& h, UInt8& s, UInt8& v)
  {
    float max = Math::Max(r, Math::Max(g, b));
    float min = Math::Min(r, Math::Min(g, b));
    float delta = max - min;

    v = static_cast<UInt8>(max * 255.0f);
    if (delta < 0.00001f)
    {
      h = 0;
      s = 0;
      return;
    }
    s = static_cast<UInt8>((delta / max) * 255.0f);
    if (r >= max)
      h = static_cast<UInt8>(0 + (g - b) / delta * 42.5f);
    else if (g >= max)
      h = static_cast<UInt8>(85 + (b - r) / delta * 42.5f);
    else
      h = static_cast<UInt8>(170 + (r - g) / delta * 42.5f);
  }

  inline void Color::getHSV(float& h, float& s, float& v)
  {
    UInt8 hue, sat, val;
    getHSV(hue, sat, val);
    h = hue / 255.0f;
    s = sat / 255.0f;
    v = val / 255.0f;
  }

  inline void Color::getRGB(UInt8& rOut, UInt8& gOut, UInt8& bOut)
  {
    rOut = static_cast<UInt8>(Math::Min(Math::Max(r * 255.0f, 0.0f), 255.0f));
    gOut = static_cast<UInt8>(Math::Min(Math::Max(g * 255.0f, 0.0f), 255.0f));
    bOut = static_cast<UInt8>(Math::Min(Math::Max(b * 255.0f, 0.0f), 255.0f));
  }

  inline void Color::getRGBA(UInt8& rOut, UInt8& gOut, UInt8& bOut, UInt8& aOut)
  {
    getRGB(rOut, gOut, bOut);
    aOut = static_cast<UInt8>(Math::Min(Math::Max(a * 255.0f, 0.0f), 255.0f));
  }

  inline void Color::clamp()
  {
    r = Math::Min(Math::Max(r, 0.0f), 1.0f);
    g = Math::Min(Math::Max(g, 0.0f), 1.0f);
    b = Math::Min(Math::Max(b, 0.0f), 1.0f);
    a = Math::Min(Math::Max(a, 0.0f), 1.0f);
  }

  // scalar multiplication from left
  constexpr Color operator*(float scalar, const Color& color)
  {
    return color * scalar;
  }
}
