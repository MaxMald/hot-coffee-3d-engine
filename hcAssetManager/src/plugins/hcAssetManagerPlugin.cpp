#include "hc/plugins/hcAssetManagerPlugin.h"
#include "hc/assets/hcAssetManager.h"

namespace hc
{
  HC_ASSET_MANAGER_EXPORT IPlugin* createAssetManagerPlugin()
  {
    return new AssetManagerPlugin();
  }

  HC_ASSET_MANAGER_EXPORT void destroyAssetManagerPlugin()
  {
  }

  void AssetManagerPlugin::onConnect()
  {
  }

  void AssetManagerPlugin::onClose()
  {
  }

  UniquePtr<IAssetManager> AssetManagerPlugin::createAssetManager() const
  {
    return MakeUnique<AssetManager>();
  }
}
