#pragma once

#include "hc/editor/hcATypedAssetGroupDrawer.h"

namespace hc::editor
{
  class ImageAssetGroupDrawer : public ATypedAssetGroupDrawer<Image>
  {
  public:
    ImageAssetGroupDrawer(AssetManager& assetManager);
    virtual ~ImageAssetGroupDrawer() = default;

  protected:
    void drawAssetDetails(const SharedPtr<Image>& asset) override;
  };
}
