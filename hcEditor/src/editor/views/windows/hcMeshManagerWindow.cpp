#include "hc/editor/views/windows/hcMeshManagerWindow.h"
#include "imgui.h"

namespace hc::editor
{
  MeshManagerWindow::MeshManagerWindow(IMeshManager& meshManager) :
    AWindowView("Mesh Manager", false),
    m_meshManager(meshManager)
  {
  }

  void MeshManagerWindow::destroy()
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

    String path = mesh->getSourcePath().empty() ? "N/A" : mesh->getSourcePath().generic_string();
    ImGui::Text("Source Path: %s", path.c_str());
    ImGui::Separator();
    if (ImGui::TreeNode("Materials"))
    {
      drawMaterialsInfo(materials);
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
}
