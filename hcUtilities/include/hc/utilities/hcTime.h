#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"

namespace hc
{
  /**
   * @brief Represents a time duration with microsecond precision.
   *
   * The Time class provides utility functions to convert between different time
   * units and to access the underlying microsecond representation.
   */
  class HC_UTILITY_EXPORT Time
  {
  public:
    /**
     * @brief Default constructor. Initializes the time duration to zero.
     */
    Time();

    /**
     * @brief Constructs a Time object from a microsecond duration.
     *
     * @param microseconds The duration in microseconds.
     */
    Time(const std::chrono::microseconds& microseconds);

    /**
     * @brief Returns the time duration in seconds as a floating-point value.
     * 
     * @return Duration in seconds.
     */
    float toSeconds() const;

    /**
     * @brief Returns the time duration in milliseconds as a 32-bit integer.
     * 
     * @return Duration in milliseconds.
     */
    Int32 toMilliseconds() const;

    /**
     * @brief Returns the time duration in microseconds as a 64-bit integer.
     * 
     * @return Duration in microseconds.
     */
    Int64 toMicroseconds() const;

    /**
     * @brief Returns the time duration as a std::chrono::microseconds object.
     * 
     * @return Duration as std::chrono::microseconds.
     */
    std::chrono::microseconds toChronoMicroseconds() const;

  private:
    std::chrono::microseconds m_microseconds;
  };
}
