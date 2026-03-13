#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcMeshComponentDrawer.h"
#include "hc/editor/views/hcProjectFileSelectorView.h"
#include "imgui.h"

namespace hc::editor
{
  MeshComponentDrawer::MeshComponentDrawer(
    IMeshManager& meshManager,
    ProjectFileSelectorView& projectFileSelector
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

        String label = String::Format("Material Slot %u", i);
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

    ImGui::Text("Asset Id: %u", material->getId().value());
    ImGui::Text("Material ID: %d", material->getMaterialId());
    ImGui::Text("Shader Type: %s", shadingType::toString(material->getShaderType()).c_str());

    SharedPtr<AMaterialDescriptor> descriptor = material->getDescriptor();
    if (!descriptor)
      return;

    materialRenderMode::Type currentRenderMode = descriptor->getRenderMode();
    const char* renderModeOptions[] = { "Background", "Opaque", "AlphaCutout", "Transparent" };
    int currentItem = static_cast<int>(currentRenderMode);

    if (ImGui::Combo("Render Mode", &currentItem, renderModeOptions, 4))
      descriptor->setRenderMode(static_cast<materialRenderMode::Type>(currentItem));

    bool isTwoSided = descriptor->isDoubleSided();
    if (ImGui::Checkbox("Two-Sided", &isTwoSided))
      descriptor->setDoubleSided(isTwoSided);

    if (descriptor->getRenderMode() == materialRenderMode::Type::AlphaCutout)
    {
      float alphaCutoff = descriptor->getAlphaCutoutThreshold();
      if (ImGui::SliderFloat("Alpha Cutoff", &alphaCutoff, 0.0f, 1.0f))
        descriptor->setAlphaCutoutThreshold(alphaCutoff);
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
