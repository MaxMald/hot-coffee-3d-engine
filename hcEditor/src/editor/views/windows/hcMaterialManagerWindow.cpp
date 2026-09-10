#include "hc/editor/views/windows/hcMaterialManagerWindow.h"

#include "imgui.h"

#include "hc/editor/services/materialDrawer/hcMaterialDrawersManager.h"

namespace hc::editor
{
  MaterialManagerWindow::MaterialManagerWindow(
    IMaterialManager& materialManager,
    MaterialDrawersManager& materialDrawersManager
  ) : AWindowView("Material Manager"),
    m_materialManager(materialManager),
    m_materialDrawersManager(materialDrawersManager)
  {}

  MaterialManagerWindow::~MaterialManagerWindow()
  {}

  void MaterialManagerWindow::destroy()
  {}

  void MaterialManagerWindow::onDraw()
  {
    const Vector<SharedPtr<IMaterial>>& materials = m_materialManager.getMaterials();
    if (materials.empty())
    {
      ImGui::Text("No materials loaded.");
      return;
    }

    for (const SharedPtr<IMaterial>& material : materials)
    {
      if (!material)
        return;

      String materialIdStr = String::Format(
        "Material UUID: %s", material->getUUID().toString().c_str()
      );

      if (ImGui::TreeNode(materialIdStr.c_str()))
      {
        m_materialDrawersManager.drawMaterial(material.get());
        ImGui::TreePop();
      }
      ImGui::Separator();
    }
  }
}
