#include "hc/hcAssetManager.h"

namespace hc
{
  void AssetManager::clear()
  { 
    m_assetGroups.clear();
    m_assetLoaders.clear();
  }
}
