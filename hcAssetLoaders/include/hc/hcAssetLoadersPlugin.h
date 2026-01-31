#pragma once

#include "hc/hcAssetLoadersPrerequisites.h"

namespace hc
{
  extern "C"
  {
    HC_ASSET_LOADERS_EXPORT IPlugin* createAssetLoadersPlugin();
    HC_ASSET_LOADERS_EXPORT void destroyAssetLoadersPlugin();
  }

  class HC_ASSET_LOADERS_EXPORT AssetLoadersPlugin : public IPlugin
  {
  public:
    AssetLoadersPlugin();
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
    * @copydoc IPlugin::getData
    */
    void* getData() override;

    /**
    * @copydoc IPlugin::addDependencies
    */
    void addDependencies(DependencyContainer& container) override;
  };
}
