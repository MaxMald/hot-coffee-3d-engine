#include "hc/editor/views/windows/assetManagerWindow/assetManagerDrawer/hcMaterialDescriptorAssetManagerDrawer.h"
#include <imgui.h>

namespace hc::editor
{
  MaterialDescriptorAssetManagerDrawer::MaterialDescriptorAssetManagerDrawer(
    IMaterialDescriptorAssetManager& assetManager
  ) :
    AAssetManagerDrawer<MaterialDescriptor>(assetManager)
  {
  }

  void MaterialDescriptorAssetManagerDrawer::drawAssetDetails(
    const SharedPtr<MaterialDescriptor>& asset
  )
  {
    String materialTypeStr = materialType::toString(asset->getType());
    ImGui::Text("Material Type: %s", materialTypeStr.c_str());
  }
}
