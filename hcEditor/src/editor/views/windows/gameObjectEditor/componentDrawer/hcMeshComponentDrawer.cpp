#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcMeshComponentDrawer.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"
#include "imgui.h"

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

        String label = String::Format("Material Slot %d", i);
        if (ImGui::TreeNode(label.c_str()))
        {
          drawMaterialInformation(material);
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
  }

  void MeshComponentDrawer::drawMaterialInformation(const SharedPtr<IMaterial>& material)
  {
    if (!material)
      return;

    ImGui::Text("Asset Id: %llu", material->getId().value());
    ImGui::Text("Material ID: %u", material->getMaterialId());
    ImGui::Text("Shader Type: %s", shadingType::toString(material->getShaderType()).c_str());

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
