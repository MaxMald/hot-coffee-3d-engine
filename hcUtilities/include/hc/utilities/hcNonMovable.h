#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"

namespace hc
{
  /**
   * @brief Utility base class to prevent move construction and move assignment.
   *
   * Inherit from NonMovable to make a class non-movable.
   */
  class HC_UTILITY_EXPORT NonMovable
  {
  protected:
    NonMovable() = default;
    ~NonMovable() = default;

    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;
  };
}
