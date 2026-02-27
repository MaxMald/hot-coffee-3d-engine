#pragma once

#include <hc/plugins/hcIAssetManagerPlugin.h>
#include "hc/hcAssetManagerPrerequisites.h"

namespace hc
{
  extern "C"
  {
    HC_ASSET_MANAGER_EXPORT IPlugin* createAssetManagerPlugin();
    HC_ASSET_MANAGER_EXPORT void destroyAssetManagerPlugin();
  }

  class HC_ASSET_MANAGER_EXPORT AssetManagerPlugin : public IAssetManagerPlugin
  {
  public:
    AssetManagerPlugin() = default;
    virtual ~AssetManagerPlugin() = default;
    /**
     * @copydoc IPlugin::onConnect
     */
    void onConnect() override;

    /**
     * @copydoc IPlugin::onClose
     */
    void onClose() override;

    /**
     * @copydoc IAssetLoaderPlugin::registerAssetLoaders
     */
    void registerAssetLoaders(AssetManager& assetManager) const override;
  };
}
