#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcMeshComponentDrawer.h"
#include <imgui.h>
#include "hc/editor/imgui/hcImguiUtilities.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"
#include "hc/editor/services/materialDrawer/hcMaterialDrawersManager.h"

namespace hc::editor
{
  MeshComponentDrawer::MeshComponentDrawer(
    IMeshManager& meshManager,
    IAssetManager& assetManager,
    ProjectFileDialogView& projectFileSelector,
    MaterialDrawersManager& materialDrawerManager
  ) : 
    ABaseComponentDrawer<MeshComponent>(componentType::Mesh),
    m_meshManager(meshManager),
    m_assetManager(assetManager),
    m_projectFileSelector(projectFileSelector),
    m_materialDrawerManager(materialDrawerManager)
  {}

  MeshComponentDrawer::~MeshComponentDrawer()
  {}

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
      ImGui::Text("Mesh UUID: %s", mesh->getUUID().toString().c_str());
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

  void MeshComponentDrawer::drawMaterialsInformation(
    const Vector<SharedPtr<IMaterial>>& materials
  )
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
          m_materialDrawerManager.drawMeshMaterial(material.get(), i);
          ImGui::TreePop();
        }
        ImGui::PopID();
      }
      ImGui::TreePop();
    }
  }

  void MeshComponentDrawer::onMeshFileSelected(
    MeshComponent* component,
    const Path& selectedPath
  )
  {
    if (selectedPath.empty() || !component)
      return;

    SharedPtr<IMesh> mesh =  m_meshManager.createMeshFromPath(selectedPath);

    Path sourcePath = selectedPath;
    if (selectedPath.isAbsolute() && m_assetManager.hasRootPath())
      sourcePath = selectedPath.toRelative(m_assetManager.getRootPath());

    component->setMesh(mesh, sourcePath);
  }
}
