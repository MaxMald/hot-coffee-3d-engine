#include "hc/editor/services/materialDrawer/hcHairMaterialDrawer.h"
#include <imgui.h>
#include "hc/editor/imgui/hcImguiUtilities.h"
#include "hc/editor/hcEditorCommons.h"

namespace hc::editor
{
  void HairMaterialDrawer::onDraw(HairMaterial* material)
  {
    if (!material)
      return;

    // TODO
  }

  void HairMaterialDrawer::onDrawMeshMaterial(
    HairMaterial* material,
    Int32 slotIndex
  )
  {
    (void)slotIndex; // unused parameter

    Color specularPrimaryColor = material->getSpecularPrimaryColor();
    if (imguiUtilities::DrawColorEdit3("Primary Specular Color", specularPrimaryColor))
      material->setSpecularPrimaryColor(specularPrimaryColor);

    Color specularSecondaryColor = material->getSpecularSecondaryColor();
    if (imguiUtilities::DrawColorEdit3("Secondary Specular Color", specularSecondaryColor))
      material->setSpecularSecondaryColor(specularSecondaryColor);

    float shininess = material->getShininess();
    if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 256.0f))
      material->setShininess(shininess);

    float specularPrimaryShift = material->getSpecularPrimaryShift();
    if (ImGui::SliderFloat("Specular Primary Shift", &specularPrimaryShift, -4.0f, 4.0f))
      material->setSpecularPrimaryShift(specularPrimaryShift);

    float specularSecondaryShift = material->getSpecularSecondaryShift();
    if (ImGui::SliderFloat("Specular Secondary Shift", &specularSecondaryShift, -4.0f, 4.0f))
      material->setSpecularSecondaryShift(specularSecondaryShift);

    float specularStrength = material->getSpecularStrength();
    if (ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f))
      material->setSpecularStrength(specularStrength);

    float specularWidth = material->getSpecularWidth();
    if (ImGui::SliderFloat("Specular Width", &specularWidth, 0.0f, 1.0f))
      material->setSpecularWidth(specularWidth);

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
