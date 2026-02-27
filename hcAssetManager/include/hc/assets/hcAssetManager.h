#pragma once

#include "hc/hcAssetManagerPrerequisites.h"
#include "hc/assets/model/hcModelAssetManager.h"
#include "hc/assets/image/hcImageAssetManager.h"
#include "hc/assets/materialDescriptor/hcMaterialDescriptorAssetManager.h"

namespace hc
{
  class HC_ASSET_MANAGER_EXPORT AssetManager : public IAssetManager
  {
  public:
    AssetManager();
    virtual ~AssetManager();

    IModelAssetManager& getModelAssetManager() override;
    IImageAssetManager& getImageAssetManager() override;
    IMaterialDescriptorAssetManager& getMaterialDescriptorAssetManager() override;
    void clear() override;
    void destroy() override;

  private:
    ModelAssetManager m_modelAssetManager;
    ImageAssetManager m_imageAssetManager;
    MaterialDescriptorAssetManager m_materialDescriptorAssetManager;
  };
}
