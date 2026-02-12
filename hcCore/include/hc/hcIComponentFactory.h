#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Interface for component factories.
   *
   * Provides a base interface for all component factory types. Enables
   * polymorphic destruction and enforces a common base for factory
   * implementations.
   */
  class IComponentFactory
  {
  public:
    virtual ~IComponentFactory() = default;

  protected:
    IComponentFactory() = default;
  };
}
