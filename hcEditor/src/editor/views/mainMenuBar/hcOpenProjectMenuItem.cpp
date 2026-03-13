#include "hc/editor/views/mainMenuBar/hcOpenProjectMenuItem.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/views/fileDialog/hcFileDialogView.h"
#include "hc/editor/views/fileDialog/hcFileDialogRequest.h"
#include "imgui.h"

namespace hc::editor
{
  static constexpr const char* PROJECT_FILE_EXTENSION = ".hotCoffeeProj";
  static constexpr const char* OPEN_PROJECT_DIALOG_KEY = "OpenProject";

  OpenProjectMenuItem::OpenProjectMenuItem(
    ProjectManager& projectManager,
    FileDialogView& fileDialogView
  ) :
    m_projectManager(projectManager),
    m_fileDialogView(fileDialogView)
  {
  }

  void OpenProjectMenuItem::draw()
  {
    if (ImGui::MenuItem("Open Project"))
    {
      if (m_fileDialogView.hasRequest(OPEN_PROJECT_DIALOG_KEY))
        return;

      IGFD::FileDialogConfig config;
      config.path = ".";

      FileDialogRequest request(
        OPEN_PROJECT_DIALOG_KEY,
        "Choose Project",
        PROJECT_FILE_EXTENSION,
        this,
        config
      );

      m_fileDialogView.openFileDialog(request);
    }
  }

  void OpenProjectMenuItem::onFileSelected(
    const String& dialogKey,
    const Path& filePathName,
    const Path& currentPath
  )
  {
    if (dialogKey == OPEN_PROJECT_DIALOG_KEY)
      m_projectManager.openProject(filePathName);
  }
}
