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
    static constexpr float Epsilon = 1e-6f;

    template<typename T>
    static inline constexpr T Min(T a, T b)
    {
      return std::min(a, b);
    }

    template<typename T>
    static inline constexpr T Max(T a, T b)
    {
      return std::max(a, b);
    }

    template<typename T>
    static inline constexpr T Clamp(T value, T minValue, T maxValue)
    {
      return Min(Max(value, minValue), maxValue);
    }

    template<typename T>
    static inline constexpr T Abs(T value)
    {
      return std::abs(value);
    }

    template<typename T>
    static constexpr T Lerp(T a, T b, float t)
    {
      return a + (b - a) * t;
    }

    static inline constexpr float Sign(float value)
    {
      return (value > 0.0f) ? 1.0f : ((value < 0.0f) ? -1.0f : 0.0f);
    }

    static inline float Sin(float radians)
    {
      return std::sin(radians);
    }

    static inline float Cos(float radians)
    {
      return std::cos(radians);
    }

    static inline float Tan(float radians)
    {
      return std::tan(radians);
    }

    static inline float Sqrt(float value)
    {
      return std::sqrt(value);
    }

    static inline bool IsNearlyEqual(
      float a, float b, float epsilon = Epsilon
    )
    {
      return std::abs(a - b) <= epsilon;
    }

    static inline bool IsNearlyZero(float value, float epsilon = Epsilon)
    {
      return std::abs(value) <= epsilon;
    }

    /**
     * @brief Computes the next power of two greater than or equal to the given value.
     * @param value The input value.
     * @returns The next power of two greater than or equal to the input value.
     */
    static inline UInt32 NextPowerOfTwo(UInt32 value)
    {
      if (value == 0)
        return 1;

      value--;
      value |= value >> 1;
      value |= value >> 2;
      value |= value >> 4;
      value |= value >> 8;
      value |= value >> 16;
      return ++value;
    }

    /**
     * @brief Computes the next power of two greater than or equal to the given value.
     * @param value The input value.
     * @returns The next power of two greater than or equal to the input value.
     */
    static inline SizeT NextPowerOfTwo(SizeT value)
    {
      if (value == 0)
        return 1;
      value--;
      value |= value >> 1;
      value |= value >> 2;
      value |= value >> 4;
      value |= value >> 8;
      value |= value >> 16;
      if constexpr (sizeof(SizeT) > 4)
        value |= value >> 32; // For 64-bit SizeT
      return ++value;
    }

    /**
     * @brief Checks if a given value is a power of two.
     * @param value The input value.
     * @returns True if the value is a power of two, false otherwise.
     */
    static inline bool IsPowerOfTwo(UInt32 value)
    {
      return (value != 0) && ((value & (value - 1)) == 0);
    }

    /**
     * @brief Checks if a given value is a power of two.
     * @param value The input value.
     * @returns True if the value is a power of two, false otherwise.
     */
    static inline bool IsPowerOfTwo(SizeT value)
    {
      return (value != 0) && ((value & (value - 1)) == 0);
    }
  };
}
