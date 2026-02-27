#pragma once

#include "hc/editor/views/windows/assetManagerWindow/assetGroupDrawer/hcATypedAssetGroupDrawer.h"

namespace hc::editor
{
  class MaterialDescriptorAssetGroupDrawer :
    public ATypedAssetGroupDrawer<AMaterialDescriptor>
  {
  public:
    MaterialDescriptorAssetGroupDrawer(AssetManager& assetManager);
    virtual ~MaterialDescriptorAssetGroupDrawer() = default;

  protected:
    void drawAssetDetails(const SharedPtr<AMaterialDescriptor>& asset) override;
  };
}
