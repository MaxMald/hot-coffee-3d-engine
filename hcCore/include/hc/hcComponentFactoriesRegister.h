#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class ComponentFactoriesManager;
  class SceneManager;

  namespace componentFactoriesRegister
  {
    void registerComponentFactories(
      ComponentFactoriesManager& factoryManager,
      SceneManager& sceneManager
    );
  }
}
