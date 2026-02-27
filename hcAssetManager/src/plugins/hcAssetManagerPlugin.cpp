#include "hc/plugins/hcAssetManagerPlugin.h"
#include "hc/assets/hcImageLoader.h"
#include "hc/assets/hcModelLoader.h"

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

  void AssetManagerPlugin::registerAssetLoaders(
    AssetManager& assetManager
  ) const
  {
    assetManager.addLoader<Image>(
      MakeUnique<ImageLoader>()
    );

    assetManager.addLoader<Model>(
      MakeUnique<ModelLoader>()
    );
  }
}
