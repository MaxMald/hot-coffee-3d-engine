#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class PluginManager;
  class AssetManager;

  namespace assetManagerLoadersRegistry
  {
    /**
     * @brief Registers default asset loaders with the given asset manager.
     *
     * @param assetManager The asset manager to register loaders with.
     * @param pluginManager The plugin manager to use for resolving loader
     * dependencies.
     */
    void RegisterLoaders(
      AssetManager& assetManager,
      PluginManager& pluginManager
    );
  }
}
