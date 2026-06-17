#pragma once

#include <cmath>

#include "hc/utilities/hcUtilitiesPrerequisites.h"

namespace hc
{
  struct Math
  {
    static constexpr float Pi = 3.14159265358979323846f;
    static constexpr float TwoPi = 6.28318530717958647692f;
    static constexpr float HalfPi = 1.57079632679489661923f;
    static constexpr float InvPi = 0.31830988618379067154f; // 1 / Pi
    static constexpr float DegToRad = 0.01745329252f;
    static constexpr float RadToDeg = 57.2957795131f;

    template<typename T>
    static constexpr T Min(T a, T b)
    {
      return (a < b) ? a : b;
    }

    template<typename T>
    static constexpr T Max(T a, T b)
    {
      return (a > b) ? a : b;
    }

    template<typename T>
    static constexpr T Clamp(T value, T minValue, T maxValue)
    {
      return Min(Max(value, minValue), maxValue);
    }

    template<typename T>
    static constexpr T Abs(T value)
    {
      return std::abs(value);
    }

    template<typename T>
    static constexpr T Lerp(T a, T b, float t)
    {
      return a + (b - a) * t;
    }

    static constexpr float Sign(float value)
    {
      return (value > 0.0f) ? 1.0f : ((value < 0.0f) ? -1.0f : 0.0f);
    }

    static float Sin(float radians)
    {
      return std::sin(radians);
    }

    static float Cos(float radians)
    {
      return std::cos(radians);
    }

    static float Tan(float radians)
    {
      return std::tan(radians);
    }

    static float Sqrt(float value)
    {
      return std::sqrt(value);
    }

    static bool IsNearlyEqual(
      float a, float b, float epsilon = 1e-6f
    )
    {
      return std::abs(a - b) <= epsilon;
    }

    static bool IsNearlyZero(float value, float epsilon = 1e-6f)
    {
      return std::abs(value) <= epsilon;
    }
  };
}
