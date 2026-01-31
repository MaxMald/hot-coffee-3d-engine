#pragma once

#include "hc/editor/hcATypedAssetGroupDrawer.h"

namespace hc::editor
{
  class MaterialDescriptorAssetGroupDrawer :
    public ATypedAssetGroupDrawer<MaterialDescriptor>
  {
  public:
    MaterialDescriptorAssetGroupDrawer() = default;
    virtual ~MaterialDescriptorAssetGroupDrawer() = default;

  protected:
    void drawAssetDetails(const SharedPtr<MaterialDescriptor>& asset) override;
  };
}
