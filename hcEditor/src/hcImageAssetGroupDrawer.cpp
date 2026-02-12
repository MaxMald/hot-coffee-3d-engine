#include "hc/editor/hcImageAssetGroupDrawer.h"

namespace hc::editor
{
  ImageAssetGroupDrawer::ImageAssetGroupDrawer(AssetManager& assetManager) :
    ATypedAssetGroupDrawer<Image>(assetManager)
  {
  }

  void ImageAssetGroupDrawer::drawAssetDetails(const SharedPtr<Image>& asset)
  {
    ImGui::Text("Dimensions: %ux%u", asset->getWidth(), asset->getHeight());
  }
}
