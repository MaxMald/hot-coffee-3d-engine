#pragma once

#include <hc/plugins/hcIAssetLoaderPlugin.h>
#include "hc/hcAssetLoadersPrerequisites.h"

namespace hc
{
  extern "C"
  {
    HC_ASSET_LOADERS_EXPORT IPlugin* createAssetLoadersPlugin();
    HC_ASSET_LOADERS_EXPORT void destroyAssetLoadersPlugin();
  }

  class HC_ASSET_LOADERS_EXPORT AssetLoadersPlugin : public IAssetLoaderPlugin
  {
  public:
    AssetLoadersPlugin() = default;
    virtual ~AssetLoadersPlugin() = default;

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
