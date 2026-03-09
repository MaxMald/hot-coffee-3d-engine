#include "hc/editor/views/windows/assetManagerWindow/assetManagerDrawer/hcMaterialDescriptorAssetManagerDrawer.h"
#include <imgui.h>

namespace hc::editor
{
  MaterialDescriptorAssetManagerDrawer::MaterialDescriptorAssetManagerDrawer  (
    IMaterialDescriptorAssetManager& assetManager
  ) :
    AAssetManagerDrawer<AMaterialDescriptor>(assetManager)
  {
  }

  void MaterialDescriptorAssetManagerDrawer::drawAssetDetails(
    const SharedPtr<AMaterialDescriptor>& asset
  )
  {
    String shaderTypeStr = shadingType::toString(asset->getShaderType());
    ImGui::Text("Shader Type: %s", shaderTypeStr.c_str());
  }
}
