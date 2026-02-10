#include "hc/hcAssetLoadersPlugin.h"
#include "hc/hcImageLoader.h"
#include "hc/hcModelLoader.h"

namespace hc
{
  HC_ASSET_LOADERS_EXPORT IPlugin* createAssetLoadersPlugin()
  {
    return new AssetLoadersPlugin();
  }

  HC_ASSET_LOADERS_EXPORT void destroyAssetLoadersPlugin(IPlugin*)
  {
  }

  void AssetLoadersPlugin::onConnect()
  {
  }

  void AssetLoadersPlugin::onClose()
  {
  }

  void AssetLoadersPlugin::addDependencies(DependencyContainer&)
  {
  }

  void AssetLoadersPlugin::registerAssetLoaders(
    AssetManager& assetManager
  ) const
  {
    assetManager.addLoader<Image>(
      MakeShared<ImageLoader>()
    );

    assetManager.addLoader<Model>(
      MakeShared<ModelLoader>()
    );
  }
}
