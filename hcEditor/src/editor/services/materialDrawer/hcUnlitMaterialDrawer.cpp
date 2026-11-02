#include "hc/editor/services/materialDrawer/hcUnlitMaterialDrawer.h"

#include "hc/editor/imgui/hcImgui.h"
#include "hc/editor/hcEditorCommons.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"

namespace hc::editor
{
  UnlitMaterialDrawer::UnlitMaterialDrawer(ITextureManager& textureManager) :
    ABaseMaterialDrawer(textureManager)
  {
  }

  void UnlitMaterialDrawer::onDraw(UnlitMaterial* material)
  {
    if (!material)
      return;
    
    hcImGui::DrawColor("Color", material->getColor());

    SharedPtr<ITexture> mainTexture = material->getMainTexture();
    if (mainTexture)
      ImGui::Text("Main Texture UUID: %s", mainTexture->getUUID().toString().c_str());
    else
      ImGui::Text("Main Texture: None");
  }

  void UnlitMaterialDrawer::onDrawMeshMaterial(
    UnlitMaterial* material,
    Int32 slotIndex,
    ProjectFileDialogView&
  )
  {
    (void)slotIndex; // Unused parameter

    Color color = material->getColor();
    if (hcImGui::DrawColorEdit3("Color", color))
      material->setColor(color);

    hcImGui::DrawTexture(
      material->getMainTexture().get(),
      style::COMPONENT_MAT_TEXTURE_SIZE,
      style::COMPONENT_MAT_TEXTURE_SIZE
    );
  }
}
