#include "hc/utilities/hcClock.h"

namespace hc
{
  Clock::Clock()
    : m_running(false)
  {
    m_startTimePoint = std::chrono::steady_clock::now();
    m_stopPoint = m_startTimePoint;
  }

  Time Clock::getElapsedTime() const
  {
    std::chrono::steady_clock::time_point endPoint = m_running
      ? std::chrono::steady_clock::now()
      : m_stopPoint;

    std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
      endPoint - m_startTimePoint
    );

    return Time(elapsed);
  }

  bool Clock::isRunning() const
  {
    return m_running;
  }

  void Clock::start()
  {
    if (!m_running)
    {
      m_startTimePoint = std::chrono::steady_clock::now();
      m_running = true;
    }
  }

  void Clock::stop()
  {
    if (m_running)
    {
      m_stopPoint = std::chrono::steady_clock::now();
      m_running = false;
    }
  }

  Time Clock::restart()
  {
    Time elapsed = getElapsedTime();
    m_startTimePoint = std::chrono::steady_clock::now();
    m_running = true;
    return elapsed;
  }

  Time Clock::reset()
  {
    Time elapsed = getElapsedTime();
    m_startTimePoint = std::chrono::steady_clock::now();
    m_stopPoint = m_startTimePoint;
    m_running = false;
    return elapsed;
  }
}
