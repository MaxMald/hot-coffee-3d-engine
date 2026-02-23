#pragma once

#include "hc/plugins/hcIPlugin.h"

namespace hc
{
  class AssetManager;

  /**
   * @brief Interface for asset loader plugins.
   *
   * Provides a contract for plugins that register asset loaders with the asset
   * manager. Implementations must provide a method to register their asset
   * loaders.
   */
  class HC_CORE_EXPORT IAssetLoaderPlugin : public IPlugin
  {
  public:
    virtual ~IAssetLoaderPlugin() = default;

    /**
     * @brief Registers asset loaders with the provided asset manager.
     *
     * @param assetManager Reference to the asset manager where loaders will be
     * registered.
     */
    virtual void registerAssetLoaders(AssetManager& assetManager) const = 0;

  protected:
    IAssetLoaderPlugin() = default;
  };
}
