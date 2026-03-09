#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IAssetManager;
  class PluginManager;

  /**
   * Helper class to access the asset manager plugin from the plugin manager.
   */
  struct AssetManagerPluginAccessor
  {
    /**
     * Retrieves the asset manager from the plugin manager.
     *
     * @param pluginManager The plugin manager to query for the asset manager plugin.
     * 
     * @return Pointer to the asset manager instance.
     * 
     * @throws RuntimeErrorException if the asset manager plugin is not found or does not
     * implement IAssetManagerPlugin.
     */
    static IAssetManager* GetAssetManager(PluginManager& pluginManager);
  };
}
