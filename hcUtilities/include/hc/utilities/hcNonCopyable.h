#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"

namespace hc
{
  /**
   * @brief Utility base class to prevent copy construction.
   *
   * Inherit from NonCopyable to make a class non-copyable.
   */
  class HC_UTILITY_EXPORT NonCopyable
  {
  protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
  };
}
