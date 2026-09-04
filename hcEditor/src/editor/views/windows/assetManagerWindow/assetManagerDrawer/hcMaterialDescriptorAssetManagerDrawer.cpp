#include "hc/editor/views/windows/assetManagerWindow/assetManagerDrawer/hcMaterialDescriptorAssetManagerDrawer.h"
#include <imgui.h>

namespace hc::editor
{
  MaterialDescriptorAssetManagerDrawer::MaterialDescriptorAssetManagerDrawer(
    IMaterialDescriptorAssetManager& assetManager
  ) :
    AAssetManagerDrawer<AMaterialDescriptor>(assetManager)
  {
  }

  void MaterialDescriptorAssetManagerDrawer::drawAssetDetails(
    const SharedPtr<AMaterialDescriptor>& asset
  )
  {
    String materialTypeStr = materialType::toString(asset->getType());
    ImGui::Text("Material Type: %s", materialTypeStr.c_str());
  }
}
