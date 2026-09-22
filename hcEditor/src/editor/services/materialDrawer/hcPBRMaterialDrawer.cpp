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

    ImGui::Text("Materials");
    ImGui::Separator();

    if (ImGui::BeginTable("Textures", 3, ImGuiTableFlags_SizingFixedFit))
    {
      ImGui::TableNextRow(); // Labels

      ImGui::TableSetColumnIndex(0);
      ImGui::Text("Albedo");

      ImGui::TableSetColumnIndex(1);
      ImGui::Text("Normal");

      ImGui::TableSetColumnIndex(2);
      ImGui::Text("ORM");

      ImGui::TableNextRow(); // Buttons

      ImGui::TableSetColumnIndex(0);
      ImGui::PushID("AlbedoLoadButton");
      if (ImGui::Button("Load"))
      {
        // Handle albedo texture selection
      }
      ImGui::PopID();

      ImGui::TableSetColumnIndex(1);
      ImGui::PushID("NormalLoadButton");
      if (ImGui::Button("Load"))
      {
        // Handle normal texture selection
      }
      ImGui::PopID();

      ImGui::TableSetColumnIndex(2);
      ImGui::PushID("ORMLoadButton");
      if (ImGui::Button("Load"))
      {
        // Handle ORM texture selection
      }
      ImGui::PopID();

      // Row 2: Textures
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);

      imguiUtilities::DrawTexture(
        material->getAlbedoTexture().get(),
        style::COMPONENT_MAT_TEXTURE_SIZE,
        style::COMPONENT_MAT_TEXTURE_SIZE
      );

      ImGui::TableSetColumnIndex(1);
      imguiUtilities::DrawTexture(
        material->getNormalTexture().get(),
        style::COMPONENT_MAT_TEXTURE_SIZE,
        style::COMPONENT_MAT_TEXTURE_SIZE
      );

      ImGui::TableSetColumnIndex(2);
      imguiUtilities::DrawTexture(
        material->getORMTexture().get(),
        style::COMPONENT_MAT_TEXTURE_SIZE,
        style::COMPONENT_MAT_TEXTURE_SIZE
      );

      ImGui::EndTable();
    }
  }
}
