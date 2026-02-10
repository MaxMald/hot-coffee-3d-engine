#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class MaterialFactoriesManager;

  namespace materialFactoriesManagerRegistry
  {
    /**
     * @brief Registers material factories with the provided
     * MaterialFactoriesManager.
     *
     * This function is responsible for registering all material factories with
     * the provided MaterialFactoriesManager instance.
     */
    void HC_CORE_EXPORT resigtryMaterialFactories(
      MaterialFactoriesManager& materialFactoriesManager
    );
  }
}
