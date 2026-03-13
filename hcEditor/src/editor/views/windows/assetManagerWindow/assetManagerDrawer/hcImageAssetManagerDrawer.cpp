#include "hc/editor/views/windows/assetManagerWindow/assetManagerDrawer/hcImageAssetManagerDrawer.h"
#include <imgui.h>

namespace hc::editor
{
  ImageAssetManagerDrawer::ImageAssetManagerDrawer(
    IImageAssetManager& imageAssetManager
  ) :
    AAssetManagerDrawer<Image>(imageAssetManager)
  {
  }

  void ImageAssetManagerDrawer::drawAssetDetails(
    const SharedPtr<Image>& asset
  )
  {
    ImGui::Text("Dimensions: %ux%u", asset->getWidth(), asset->getHeight());
  }
}
