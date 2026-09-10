#include "hc/editor/services/materialDrawer/hcUnlitMaterialDrawer.h"

#include <imgui.h>

#include "hc/editor/imgui/hcImguiUtilities.h"
#include "hc/editor/hcEditorCommons.h"


namespace hc::editor
{
  void UnlitMaterialDrawer::onDraw(UnlitMaterial* material)
  {
    if (!material)
      return;
    
    imguiUtilities::DrawColor("Color", material->getColor());

    SharedPtr<ITexture> mainTexture = material->getMainTexture();
    if (mainTexture)
      ImGui::Text("Main Texture UUID: %s", mainTexture->getUUID().toString().c_str());
    else
      ImGui::Text("Main Texture: None");
  }

  void UnlitMaterialDrawer::onDrawMeshMaterial(UnlitMaterial* material, Int32 slotIndex)
  {
    (void)slotIndex; // Unused parameter

    Color color = material->getColor();
    if (imguiUtilities::DrawColorEdit3("Color", color))
      material->setColor(color);

    imguiUtilities::DrawTexture(
      material->getMainTexture().get(),
      style::COMPONENT_MAT_TEXTURE_SIZE,
      style::COMPONENT_MAT_TEXTURE_SIZE
    );
  }
}
