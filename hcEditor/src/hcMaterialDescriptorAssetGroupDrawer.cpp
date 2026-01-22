#include "hc/editor/hcMaterialDescriptorAssetGroupDrawer.h"

namespace hc::editor
{
  void MaterialDescriptorAssetGroupDrawer::drawAssetDetails(
    const SharedPtr<MaterialDescriptor>& asset
  )
  {
    String shaderTypeStr = shaderType::toString(asset->getShaderType());
    ImGui::Text("Shader Type: %s", shaderTypeStr.c_str());
  }
}
