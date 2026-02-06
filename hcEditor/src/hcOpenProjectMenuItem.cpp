#include "hc/editor/hcOpenProjectMenuItem.h"
#include "hc/editor/hcProjectManager.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

namespace hc::editor
{
  static constexpr const char* PROJECT_FILE_EXTENSION = ".hotCoffeeProj";
  static constexpr const char* OPEN_PROJECT_DIALOG_KEY = "OpenProject";

  OpenProjectMenuItem::OpenProjectMenuItem(ProjectManager& projectManager) :
    m_projectManager(projectManager)
  {
  }

  void OpenProjectMenuItem::draw()
  {
    if (ImGui::MenuItem("Open Project"))
    {
      IGFD::FileDialogConfig config;
      config.path = ".";

      ImGuiFileDialog::Instance()->OpenDialog(
        OPEN_PROJECT_DIALOG_KEY,
        "Choose Project",
        PROJECT_FILE_EXTENSION,
        config
      );
    }

    if (shouldDrawDialog())
      drawDialog();
  }

  bool OpenProjectMenuItem::shouldDrawDialog() const
  {
    return ImGuiFileDialog::Instance()->IsOpened(OPEN_PROJECT_DIALOG_KEY);
  }

  void OpenProjectMenuItem::drawDialog()
  {
    if (ImGuiFileDialog::Instance()->IsOk())
    {
      String filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      String filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      m_projectManager.openProject(Path(filePathName.c_str()));
    }
    ImGuiFileDialog::Instance()->Close();
  }
}
