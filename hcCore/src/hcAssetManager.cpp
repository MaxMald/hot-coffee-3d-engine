#include "hc/hcAssetManager.h"

#include "hc/hcDependencyContainer.h"
#include "hc/hcITextureLoader.h"

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
    SharedPtr<ITextureLoader> textureLoader = container.resolve<ITextureLoader>();
    m_assetLoaders[typeid(Texture)] = textureLoader;
  }

  void AssetManager::clear()
  {
    m_assetGroups.clear();
  }
}
