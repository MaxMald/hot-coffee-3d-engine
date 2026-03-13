#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class MaterialFactoriesManager;

  /**
   * Factory for creating material factories manager instances.
   */
  struct HC_CORE_EXPORT MaterialFactoriesManagerFactory
  {
    /**
     * Creates a new material factories manager instance.
     *
     * @return A unique pointer to the created material factories manager.
     */
    static UniquePtr<MaterialFactoriesManager> Create();
  };
}
