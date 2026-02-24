#include "hc/assets/hcAssetManager.h"

namespace hc
{
  void AssetManager::clear()
  { 
    m_assetGroups.clear();
  }

  void AssetManager::destroy()
  {
    clear();
    m_assetLoaders.clear();
  }
}
