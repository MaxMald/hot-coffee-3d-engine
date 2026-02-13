#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class ComponentFactoriesManager;

  namespace componentFactoriesManagerFactory
  {
    UniquePtr<ComponentFactoriesManager> create();
  }
}
