#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class SceneManager;
  class ComponentFactoriesManager;

  namespace componentFactoriesManagerRegistry
  {
    void registerFactories(
      ComponentFactoriesManager& manager,
      SceneManager& sceneManager
    );
  }
}
