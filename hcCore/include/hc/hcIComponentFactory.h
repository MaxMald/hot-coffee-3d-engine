#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Interface for factories that create component instances.
   */
  class IComponentFactory
  {
  public:
    virtual ~IComponentFactory() = default;

  protected:
    IComponentFactory() = default;
  };
}
