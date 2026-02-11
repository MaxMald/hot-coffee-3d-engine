#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Base interface for asset loader objects.
   *
   * Provides a contract for asset loader implementations. All asset loaders must
   * inherit from this interface to ensure proper polymorphic destruction.
   */
  class HC_CORE_EXPORT IAssetLoader
  {
  public:
    virtual ~IAssetLoader() = default;

  protected:
    IAssetLoader() = default;
  };
}
