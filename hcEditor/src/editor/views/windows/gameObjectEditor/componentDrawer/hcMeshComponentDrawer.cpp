#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcMeshComponentDrawer.h"
#include <imgui.h>
#include "hc/editor/imgui/hcImguiUtilities.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"
#include "hc/editor/services/materialDrawer/hcMaterialDrawersManager.h"
#include "hc/editor/services/metadataManager/hcEditorMetadataManager.h"

namespace hc::editor
{
  MeshComponentDrawer::MeshComponentDrawer(
    IMeshManager& meshManager,
    IAssetManager& assetManager,
    ProjectFileDialogView& projectFileSelector,
    MaterialDrawersManager& materialDrawerManager,
    EditorMetadataManager& editorMetadataManager
  ) : 
    ABaseComponentDrawer<MeshComponent>(componentType::Mesh),
    m_meshManager(meshManager),
    m_assetManager(assetManager),
    m_projectFileSelector(projectFileSelector),
    m_materialDrawerManager(materialDrawerManager),
    m_editorMetadataManager(editorMetadataManager)
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

    drawMaterialsInformation(*mesh);
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
    const IMesh& mesh
  )
  {
    const Vector<SharedPtr<IMaterial>>& materials = mesh.getMaterials();

    if (ImGui::TreeNode("Materials Information"))
    {
      for (SizeT i = 0; i < materials.size(); ++i)
      {
        SharedPtr<IMaterial> material = materials[i];
        if (!material)
          continue;

        ImGui::PushID(static_cast<Int32>(i));
        String name = material->getName();
        if (ImGui::TreeNode(name.c_str()))
        {
          m_materialDrawerManager.drawMeshMaterial(material.get(), i);

          if (ImGui::Button("Override Material"))
            onOverrideMaterialClicked(mesh, material, i);

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

  void MeshComponentDrawer::onOverrideMaterialClicked(
    const IMesh& mesh,
    const SharedPtr<IMaterial> material,
    SizeT materialIndex
  )
  {
    Path meshSourcePath = mesh.getSourcePath();
    if (meshSourcePath.empty())
    {
      LogService::Error(
        String::Format(
          "MeshComponentDrawer::onSaveMaterialClicked: Mesh does not have a source path. Cannot save material override."
        )
      );
      return;
    }

    bool success = m_editorMetadataManager.getModelMetadataManager().saveMaterialForOverride(
      meshSourcePath,
      material->getName(),
      materialIndex,
      material
    );

    if (success)
    {
      LogService::Message(
        String::Format(
          "Successfully saved material override for model '%s', material name '%s'",
          meshSourcePath.toGenericString().c_str(),
          material->getName().c_str()
        )
      );
    }
  }
}
