#pragma once

#include "hc/editor/views/windows/assetManagerWindow/assetGroupDrawer/hcAAssetManagerDrawer.h"

namespace hc::editor
{
  class MaterialDescriptorAssetManagerDrawer :
    public AAssetManagerDrawer<AMaterialDescriptor>
  {
  public:
    MaterialDescriptorAssetManagerDrawer(
      IMaterialDescriptorAssetManager& assetManager
    );
    virtual ~MaterialDescriptorAssetManagerDrawer() = default;

  protected:
    void drawAssetDetails(const SharedPtr<AMaterialDescriptor>& asset) override;
  };
}
