#include "hc/editor/views/windows/hcMeshManagerWindow.h"
#include "imgui.h"

namespace hc::editor
{
  MeshManagerWindow::MeshManagerWindow(IMeshManager& meshManager) :
    AWindowView("Mesh Manager", false),
    m_meshManager(meshManager)
  {
  }

  void MeshManagerWindow::onDraw()
  {
    const Vector<SharedPtr<IMesh>>& meshes = m_meshManager.getMeshes();

    if (ImGui::TreeNode("Meshes"))
    {
      for (const SharedPtr<IMesh>& mesh : meshes)
      {
        String label = String::Format("Mesh ID: %s", mesh->getId().toString().c_str());
        if (ImGui::TreeNode(label.c_str()))
        {
          drawMeshInfo(mesh);
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
  }

  void MeshManagerWindow::drawMeshInfo(const SharedPtr<IMesh>& mesh)
  {
    const Vector<SharedPtr<IMaterial>>& materials = mesh->getMaterials();
    const Vector<ModelSubMesh>& subMeshes = mesh->getModel()->getSubMeshes();
    
    ImGui::Text("Model ID: %s", mesh->getModel()->getId().toString().c_str());
    ImGui::Separator();
    if (ImGui::TreeNode("Materials"))
    {
      drawMaterialsInfo(materials);
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("SubMeshes"))
    {
      drawSubMeshesInfo(subMeshes);
      ImGui::TreePop();
    }
  }

  void MeshManagerWindow::drawMaterialsInfo(
    const Vector<SharedPtr<IMaterial>>& materials
  )
  {
    ImGui::BeginTable("Materials", 2, ImGuiTableFlags_Borders);

    ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 100.0f);
    ImGui::TableSetupColumn("Material ID", ImGuiTableColumnFlags_WidthFixed, 100.0f);
    ImGui::TableHeadersRow();

    for (int row = 0; row < materials.size(); ++row)
    {
      SharedPtr<IMaterial> material = materials[row];
      ImGui::TableNextRow();

      ImGui::TableSetColumnIndex(0);
      ImGui::Text("%d", row);

      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%s", material->getId().toString().c_str());
    }
    ImGui::EndTable();
  }

  void MeshManagerWindow::drawSubMeshesInfo(
    const Vector<ModelSubMesh>& subMeshes
  )
  {
    ImGui::BeginTable("SubMeshes", 6, ImGuiTableFlags_Borders);

    ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 100.0f);
    ImGui::TableSetupColumn("Material Index", ImGuiTableColumnFlags_WidthFixed, 100.0f);
    ImGui::TableSetupColumn("First Vertex Index");
    ImGui::TableSetupColumn("Vertex Count");
    ImGui::TableSetupColumn("First Index Index");
    ImGui::TableSetupColumn("Index Count");
    ImGui::TableHeadersRow();

    for (int row = 0; row < subMeshes.size(); ++row)
    {
      const ModelSubMesh& subMesh = subMeshes[row];
      ImGui::TableNextRow();

      ImGui::TableSetColumnIndex(0);
      ImGui::Text("%d", row);

      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%d", subMesh.materialIndex);

      ImGui::TableSetColumnIndex(2);
      ImGui::Text("%d", subMesh.firstVertexIndex);

      ImGui::TableSetColumnIndex(3);
      ImGui::Text("%d", subMesh.vertexCount);

      ImGui::TableSetColumnIndex(4);
      ImGui::Text("%d", subMesh.firstIndexIndex);

      ImGui::TableSetColumnIndex(5);
      ImGui::Text("%d", subMesh.indexCount);
    }
    ImGui::EndTable();
  }
}
