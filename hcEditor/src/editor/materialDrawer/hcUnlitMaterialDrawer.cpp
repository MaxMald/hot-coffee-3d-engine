#include "hc/editor/materialDrawer/hcUnlitMaterialDrawer.h"
#include "hc/editor/imgui/hcImguiUtilities.h"
#include "imgui.h"

namespace hc::editor
{
  materialType::Type UnlitMaterialDrawer::getMaterialType() const
  {
    return materialType::Unlit;
  }

  void UnlitMaterialDrawer::onDraw(UnlitMaterial* material)
  {
    if (!material)
      return;
    
    String shadingTypeStr = materialType::toString(material->getMaterialType());
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
