#include "hc/editor/services/materialDrawer/hcPBRMaterialDrawer.h"

#include <imgui.h>
#include "hc/editor/imgui/hcImguiUtilities.h"
#include "hc/editor/hcEditorCommons.h"

namespace hc::editor
{
  void PBRMaterialDrawer::onDraw(PBRMaterial* material)
  {
    if (!material)
      return;

    // TODO
  }

  void PBRMaterialDrawer::onDrawMeshMaterial(
    PBRMaterial* material,
    Int32 slotIndex
  )
  {
    (void)slotIndex; // unused parameter

    Color baseColor = material->getBaseColor();
    if (imguiUtilities::DrawColorEdit3("Base Color", baseColor))
      material->setBaseColor(baseColor);

    float metallic = material->getMetallic();
    if (ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f))
      material->setMetallic(metallic);

    float roughness = material->getRoughness();
    if (ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f))
      material->setRoughness(roughness);

    float ior = material->getIOR();
    if (ImGui::SliderFloat("Index of Refraction", &ior, Math::Epsilon, assets::materialDescriptor::MAXIMUM_IOR))
      material->setIOR(ior);

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
      material->getORMTexture().get(),
      style::COMPONENT_MAT_TEXTURE_SIZE,
      style::COMPONENT_MAT_TEXTURE_SIZE
    );
  }
}
