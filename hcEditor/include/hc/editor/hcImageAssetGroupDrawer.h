#pragma once

#include <hc/hcImage.h>
#include "hc/editor/hcATypedAssetGroupDrawer.h"

namespace hc::editor
{
  class ImageAssetGroupDrawer : public ATypedAssetGroupDrawer<Image>
  {
  public:
    ImageAssetGroupDrawer() = default;
    virtual ~ImageAssetGroupDrawer() = default;

  protected:
    void drawAssetDetails(const SharedPtr<Image>& asset) override;
  };
}
