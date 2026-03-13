#pragma once

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
    static constexpr T min(T a, T b)
    {
      return (a < b) ? a : b;
    }

    template<typename T>
    static constexpr T max(T a, T b)
    {
      return (a > b) ? a : b;
    }

    template<typename T>
    static constexpr T clamp(T value, T minValue, T maxValue)
    {
      return min(max(value, minValue), maxValue);
    }

    template<typename T>
    static constexpr T abs(T value)
    {
      return (value < T(0)) ? -value : value;
    }

    template<typename T>
    static constexpr T lerp(T a, T b, float t)
    {
      return a + (b - a) * t;
    }

    static constexpr float sign(float value)
    {
      return (value > 0.0f) ? 1.0f : ((value < 0.0f) ? -1.0f : 0.0f);
    }

    static constexpr bool isNearlyEqual(
      float a, float b, float epsilon = 1e-6f
    )
    {
      return abs(a - b) <= epsilon;
    }

    static constexpr bool isNearlyZero(float value, float epsilon = 1e-6f)
    {
      return abs(value) <= epsilon;
    }
  };
}
