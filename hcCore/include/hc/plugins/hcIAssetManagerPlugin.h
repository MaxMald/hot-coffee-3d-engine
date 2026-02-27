#pragma once

#include "hc/plugins/hcIPlugin.h"

namespace hc
{
  class AssetManager;

  /**
   * @brief Interface for asset manager plugins.
   */
  class HC_CORE_EXPORT IAssetManagerPlugin : public IPlugin
  {
  public:
    virtual ~IAssetManagerPlugin() = default;

    /**
     * @brief Registers asset loaders with the provided asset manager.
     *
     * @param assetManager Reference to the asset manager where loaders will be
     * registered.
     */
    virtual void registerAssetLoaders(AssetManager& assetManager) const = 0;

  protected:
    IAssetManagerPlugin() = default;
  };
}
