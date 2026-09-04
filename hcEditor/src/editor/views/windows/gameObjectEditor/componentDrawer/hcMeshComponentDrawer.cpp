#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcMeshComponentDrawer.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"

#include "imgui.h"
#include "hc/editor/imgui/hcImguiUtilities.h"

namespace hc::editor
{
  MeshComponentDrawer::MeshComponentDrawer(
    IMeshManager& meshManager,
    ProjectFileDialogView& projectFileSelector
  ) : 
    ABaseComponentDrawer<MeshComponent>(componentType::Mesh),
    m_meshManager(meshManager),
    m_projectFileSelector(projectFileSelector)
  {
  }

  MeshComponentDrawer::~MeshComponentDrawer()
  {
  }

  void MeshComponentDrawer::onDrawComponent(MeshComponent* component)
  {
    if (!component)
      return;

    drawLoadMeshButton(component);

    ImGui::SameLine();
    SharedPtr<IMesh> mesh = component->getMesh();
    if (!mesh)
    {
      ImGui::Text("No mesh loaded");
      return;
    }
    else
    {
      ImGui::Text("Mesh ID: %s", mesh->getId().toString().c_str());
    }

    drawMaterialsInformation(mesh->getMaterials());
  }

  void MeshComponentDrawer::drawLoadMeshButton(MeshComponent* component)
  {
    if (ImGui::Button("Load Mesh"))
    {
      m_projectFileSelector.openModelFile(
        [this, component](const Path& selectedPath)
        {
          onMeshFileSelected(component, selectedPath);
        }
      );
    }
  }

  void MeshComponentDrawer::drawMaterialsInformation(const Vector<SharedPtr<IMaterial>>& materials)
  {
    if (ImGui::TreeNode("Materials Information"))
    {
      for (Int32 i = 0; i < materials.size(); ++i)
      {
        SharedPtr<IMaterial> material = materials[i];
        if (!material)
          continue;

        ImGui::PushID(i);
        String name = material->getName();
        if (ImGui::TreeNode(name.c_str()))
        {
          drawMaterialInformation(material, i);
          ImGui::TreePop();
        }
        ImGui::PopID();
      }
      ImGui::TreePop();
    }
  }

  void MeshComponentDrawer::drawMaterialInformation(
    const SharedPtr<IMaterial>& material,
    Int32 materialSlotIndex
  )
  {
    if (!material)
      return;

    ImGui::Text("Material Slot: %d", materialSlotIndex);
    ImGui::Text("Asset Id: %llu", material->getId().value());
    ImGui::Text("Material ID: %u", material->getMaterialId());
    ImGui::Text("Material Type: %s", materialType::toString(material->getMaterialType()).c_str());

    materialRenderMode::Type currentRenderMode =  material->getRenderMode();
    const char* renderModeOptions[] = { "Background", "Opaque", "AlphaCutout", "Transparent" };
    int currentItem = static_cast<int>(currentRenderMode);

    if (ImGui::Combo("Render Mode", &currentItem, renderModeOptions, 4))
      material->setRenderMode(static_cast<materialRenderMode::Type>(currentItem));

    bool isTwoSided = material->isDoubleSided();
    if (ImGui::Checkbox("Two-Sided", &isTwoSided))
      material->setDoubleSided(isTwoSided);

    if (material->getRenderMode() == materialRenderMode::Type::AlphaCutout)
    {
      float alphaCutoff = material->getAlphaCutoutThreshold();
      if (ImGui::SliderFloat("Alpha Cutoff", &alphaCutoff, 0.0f, 1.0f))
        material->setAlphaCutoutThreshold(alphaCutoff);
    }

    // TODO
    // 
    // This is a temporary solution to expose shader-specific properties in the editor.
    // Improve this by implementing a more flexible system for material property editing
    // that can handle different shader types and their unique properties without
    // hardcoding checks for specific shader types.

    if (material->getMaterialType() == materialType::BlinnPhong)
    {
      SharedPtr<BlinnPhongMaterial> blinnPhongMaterial =
        std::dynamic_pointer_cast<BlinnPhongMaterial>(material);

      if (!blinnPhongMaterial)
        return;

      float shininess = blinnPhongMaterial->getShininess();
      if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 256.0f))
        blinnPhongMaterial->setShininess(shininess);

      imguiUtilities::DrawTexture(
        blinnPhongMaterial->getAlbedoTexture().get(),
        64.0f,
        64.0f
      );
      ImGui::SameLine();
      imguiUtilities::DrawTexture(
        blinnPhongMaterial->getNormalTexture().get(),
        64.0f,
        64.0f
      );
      ImGui::SameLine();
      imguiUtilities::DrawTexture(
        blinnPhongMaterial->getSpecularTexture().get(),
        64.0f,
        64.0f
      );
    }
    if (material->getMaterialType() == materialType::Hair)
    {
      SharedPtr<HairMaterial> hairMaterial =
        std::dynamic_pointer_cast<HairMaterial>(material);

      if (!hairMaterial)
        return;

      Color specularPrimaryColor = hairMaterial->getSpecularPrimaryColor();
      if (imguiUtilities::DrawColorEdit3("Primary Specular Color", specularPrimaryColor))
        hairMaterial->setSpecularPrimaryColor(specularPrimaryColor);

      Color specularSecondaryColor = hairMaterial->getSpecularSecondaryColor();
      if (imguiUtilities::DrawColorEdit3("Secondary Specular Color", specularSecondaryColor))
        hairMaterial->setSpecularSecondaryColor(specularSecondaryColor);

      float specularPrimaryShift = hairMaterial->getSpecularPrimaryShift();
      if (ImGui::SliderFloat("Specular Primary Shift", &specularPrimaryShift, -4.0f, 4.0f))
        hairMaterial->setSpecularPrimaryShift(specularPrimaryShift);

      float specularSecondaryShift = hairMaterial->getSpecularSecondaryShift();
      if (ImGui::SliderFloat("Specular Secondary Shift", &specularSecondaryShift, -4.0f, 4.0f))
        hairMaterial->setSpecularSecondaryShift(specularSecondaryShift);

      float shininess = hairMaterial->getShininess();
      if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 256.0f))
        hairMaterial->setShininess(shininess);

      float specularStrength = hairMaterial->getSpecularStrength();
      if (ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f))
        hairMaterial->setSpecularStrength(specularStrength);

      float specularWidth = hairMaterial->getSpecularWidth();
      if (ImGui::SliderFloat("Specular Width", &specularWidth, 0.0f, 1.0f))
        hairMaterial->setSpecularWidth(specularWidth);
    }
  }

  void MeshComponentDrawer::onMeshFileSelected(
    MeshComponent* component,
    const Path& selectedPath
  )
  {
    SharedPtr<IMesh> mesh =  m_meshManager.createMeshFromPath(selectedPath);
    component->setMesh(mesh);
  }
}
