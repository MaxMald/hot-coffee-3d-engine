#include "hc/hcAssetLoadersPlugin.h"
#include "hc/hcDependencyContainer.h"
#include "hc/hcImageLoader.h"

namespace hc
{
  HC_ASSET_LOADERS_EXPORT IPlugin* createAssetLoadersPlugin()
  {
    return new AssetLoadersPlugin();
  }

  HC_ASSET_LOADERS_EXPORT void destroyAssetLoadersPlugin(IPlugin*)
  {
  }

  AssetLoadersPlugin::AssetLoadersPlugin()
    : IPlugin()
  {
  }

  void AssetLoadersPlugin::onConnect()
  {
  }

  void AssetLoadersPlugin::onClose()
  {
  }

  void* AssetLoadersPlugin::getData()
  {
    return nullptr;
  }

  void AssetLoadersPlugin::addDependencies(DependencyContainer& container)
  {
    container.registerInstanceAsInterface<IImageLoader, ImageLoader>(
      MakeShared<ImageLoader>()
    );
  }
}
