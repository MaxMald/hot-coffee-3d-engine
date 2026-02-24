#include "hc/editor/materialDrawer/hcUnlitMaterialDrawer.h"
#include "hc/editor/imgui/hcImguiUtilities.h"
#include "imgui.h"

namespace hc::editor
{
  shadingType::Type UnlitMaterialDrawer::getShadingType() const
  {
    return shadingType::Unlit;
  }

  void UnlitMaterialDrawer::onDraw(UnlitMaterial* material)
  {
    if (!material)
      return;
    
    String shadingTypeStr = shadingType::toString(material->getShaderType());
    ImGui::Text("Shading Type: %s", shadingTypeStr.c_str());
    imguiUtilities::DrawColor("Color", material->getColor());

    SharedPtr<ITexture> mainTexture = material->getMainTexture();
    if (mainTexture)
    {
      ImGui::Text("Main Texture ID: %s", mainTexture->getId().toString().c_str());
    }
    else
    {
      ImGui::Text("Main Texture: None");
    }
  }
}
