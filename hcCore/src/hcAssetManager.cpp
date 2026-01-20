#include "hc/hcAssetManager.h"

#include "hc/hcDependencyContainer.h"
#include "hc/hcIImageLoader.h"

namespace hc
{
  AssetManager::AssetManager()
  {
  }

  AssetManager::~AssetManager()
  {
    clear();
  }

  void AssetManager::resolveDependencies(DependencyContainer& container)
  {
    SharedPtr<IImageLoader> imageLoader = container.resolve<IImageLoader>();
    m_assetLoaders[typeid(Image)] = imageLoader;
  }

  void AssetManager::clear()
  {
    m_assetGroups.clear();
  }
}
