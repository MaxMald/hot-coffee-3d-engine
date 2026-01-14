#pragma once

namespace hc
{
  /**
   * @brief Utility base class to prevent copy construction and copy assignment.
   *
   * Inherit from NonCopyable to make a class non-copyable.
   */
  class NonCopyable
  {
  protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
  };
}
