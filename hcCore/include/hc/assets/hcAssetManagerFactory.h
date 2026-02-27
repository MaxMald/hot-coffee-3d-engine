#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IAssetManager;
  class PluginManager;

  struct AssetManagerFactory
  {
    static UniquePtr<IAssetManager> Create(PluginManager& pluginManager);
  };
}
