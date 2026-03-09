#include "hc/plugins/hcAssetManagerPlugin.h"
#include "hc/assets/hcAssetManager.h"

namespace hc
{
  HC_ASSET_MANAGER_EXPORT IPlugin* createAssetManagerPlugin()
  {
    return new AssetManagerPlugin();
  }

  HC_ASSET_MANAGER_EXPORT void destroyAssetManagerPlugin(IPlugin* plugin)
  {
    if (plugin)
      delete plugin;
  }

  void AssetManagerPlugin::onConnect()
  {
  }

  void AssetManagerPlugin::onClose()
  {
    m_assetManager.destroy();
  }

  IAssetManager& AssetManagerPlugin::getAssetManager()
  {
    return m_assetManager;
  }
}
