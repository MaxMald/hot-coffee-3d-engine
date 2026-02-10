#include "hc/hcAssetManager.h"

#include "hc/hcIImageLoader.h"
#include "hc/hcIModelLoader.h"
#include "hc/hcMaterialDescriptor.h"
#include "hc/hcJsonDeserializerAssetLoader.h"

namespace hc
{
  void AssetManager::clear()
  {
    m_assetGroups.clear();
  }
}
