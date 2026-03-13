#include "hc/utilities/hcTime.h"

namespace hc
{
  Time::Time() :
    m_microseconds(0)
  {
  }

  Time::Time(const std::chrono::microseconds& microseconds) :
    m_microseconds(microseconds)
  {
  }

  float Time::toSeconds() const
  {
    return static_cast<float>(m_microseconds.count()) / 1'000'000.0f;
  }

  Int32 Time::toMilliseconds() const
  {
    return static_cast<Int32>(m_microseconds.count() / 1'000);
  }

  Int64 Time::toMicroseconds() const
  {
    return m_microseconds.count();
  }

  std::chrono::microseconds Time::toChronoMicroseconds() const
  {
    return m_microseconds;
  }
}
