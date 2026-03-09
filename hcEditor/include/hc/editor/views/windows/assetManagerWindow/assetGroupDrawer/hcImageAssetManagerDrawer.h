#pragma once

#include "hc/editor/views/windows/assetManagerWindow/assetGroupDrawer/hcAAssetManagerDrawer.h"

namespace hc::editor
{
  class ImageAssetManagerDrawer : public AAssetManagerDrawer<Image>
  {
  public:
    ImageAssetManagerDrawer(IImageAssetManager& imageAssetManager);
    virtual ~ImageAssetManagerDrawer() = default;

  protected:
    void drawAssetDetails(const SharedPtr<Image>& asset) override;
  };
}
