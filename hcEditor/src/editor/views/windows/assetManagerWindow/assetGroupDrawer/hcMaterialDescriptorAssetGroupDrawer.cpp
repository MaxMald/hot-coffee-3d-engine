#include "hc/editor/views/windows/assetManagerWindow/assetGroupDrawer/hcMaterialDescriptorAssetGroupDrawer.h"

namespace hc::editor
{
  MaterialDescriptorAssetGroupDrawer::MaterialDescriptorAssetGroupDrawer(
    AssetManager& assetManager
  ) :
    ATypedAssetGroupDrawer<AMaterialDescriptor>(assetManager)
  {
  }

  void MaterialDescriptorAssetGroupDrawer::drawAssetDetails(
    const SharedPtr<AMaterialDescriptor>& asset
  )
  {
    String shaderTypeStr = shadingType::toString(asset->getShaderType());
    ImGui::Text("Shader Type: %s", shaderTypeStr.c_str());
  }
}
