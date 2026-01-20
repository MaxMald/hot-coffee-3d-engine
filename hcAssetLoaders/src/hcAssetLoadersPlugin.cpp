#include "hc/hcAssetLoadersPlugin.h"
#include "hc/hcDependencyContainer.h"

namespace hc
{
  extern "C"
  {
    HC_ASSET_LOADERS_EXPORT IPlugin* createAssetLoadersPlugin()
    {
      return new AssetLoadersPlugin();
    }

    HC_ASSET_LOADERS_EXPORT void destroyAssetLoadersPlugin(IPlugin* plugin)
    {
    }
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
    // TODO
  }
}
