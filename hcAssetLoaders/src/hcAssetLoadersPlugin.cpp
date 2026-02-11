#include "hc/hcAssetLoadersPlugin.h"
#include "hc/hcImageLoader.h"
#include "hc/hcModelLoader.h"

namespace hc
{
  HC_ASSET_LOADERS_EXPORT IPlugin* createAssetLoadersPlugin()
  {
    return new AssetLoadersPlugin();
  }

  HC_ASSET_LOADERS_EXPORT void destroyAssetLoadersPlugin()
  {
  }

  void AssetLoadersPlugin::onConnect()
  {
  }

  void AssetLoadersPlugin::onClose()
  {
  }

  void AssetLoadersPlugin::registerAssetLoaders(
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
