#include "hc/hcAssetManager.h"

#include "hc/hcDependencyContainer.h"

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
    // TODO
  }

  void AssetManager::clear()
  {
    m_assetGroups.clear();
  }
}
