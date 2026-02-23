#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IWindowManager;
  class PluginManager;

  namespace windowManagerFactory
  {
    UniquePtr<IWindowManager> Create(
      PluginManager& pluginManager
    );
  }
}
