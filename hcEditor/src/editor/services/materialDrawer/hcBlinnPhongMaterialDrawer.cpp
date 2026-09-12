#include "hc/editor/services/materialDrawer/hcBlinnPhongMaterialDrawer.h"
#include <imgui.h>
#include "hc/editor/imgui/hcImguiUtilities.h"
#include "hc/editor/hcEditorCommons.h"

namespace hc::editor
{
  void BlinnPhongMaterialDrawer::onDraw(BlinnPhongMaterial* material)
  {
    if (!material)
      return;

    // TODO
  }

  void BlinnPhongMaterialDrawer::onDrawMeshMaterial(
    BlinnPhongMaterial* material,
    Int32 slotIndex
  )
  {
    (void)slotIndex; // unused parameter

    float shininess = material->getShininess();
    if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 256.0f))
      material->setShininess(shininess);

    imguiUtilities::DrawTexture(
      material->getAlbedoTexture().get(),
      style::COMPONENT_MAT_TEXTURE_SIZE,
      style::COMPONENT_MAT_TEXTURE_SIZE
    );
    ImGui::SameLine();
    imguiUtilities::DrawTexture(
      material->getNormalTexture().get(),
      style::COMPONENT_MAT_TEXTURE_SIZE,
      style::COMPONENT_MAT_TEXTURE_SIZE
    );
    ImGui::SameLine();
    imguiUtilities::DrawTexture(
      material->getSpecularTexture().get(),
      style::COMPONENT_MAT_TEXTURE_SIZE,
      style::COMPONENT_MAT_TEXTURE_SIZE
    );
  }
}
