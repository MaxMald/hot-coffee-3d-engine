#pragma once

#include "hc/utilities/hcTime.h"

namespace hc
{
  /**
   * @brief High-resolution clock for measuring elapsed time.
   *
   * The Clock class provides functionality to start, stop, reset, and measure
   * elapsed time using a steady clock.
   */
  class HC_UTILITY_EXPORT Clock
  {
  public:
    Clock();
    ~Clock() = default;

    /**
     * @brief Returns the elapsed time since the clock was started or last reset.
     * 
     * @return Time object representing the elapsed duration.
     */
    Time getElapsedTime() const;

    /**
     * @brief Checks if the clock is currently running.
     * 
     * @return true if the clock is running, false if stopped.
     */
    bool isRunning() const;

    /**
     * @brief Starts the clock.
     *
     * If the clock is already running, this has no effect.
     */
    void start();

    /**
     * @brief Stops the clock.
     *
     * If the clock is already stopped, this has no effect.
     */
    void stop();

    /**
     * @brief Restarts the clock and returns the elapsed time before the restart.
     * 
     * @return Time object representing the elapsed duration before restart.
     */
    Time restart();

    /**
     * @brief Resets the clock to zero and stops it.
     * 
     * @return Time object representing the elapsed duration before reset.
     */
    Time reset();

  private:
    std::chrono::steady_clock::time_point m_startTimePoint;
    std::chrono::steady_clock::time_point m_stopPoint;
    bool m_running;
  };
}
