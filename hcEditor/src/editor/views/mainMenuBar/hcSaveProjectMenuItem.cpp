#include "hc/editor/views/mainMenuBar/hcSaveProjectMenuItem.h"
#include <imgui.h>
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/views/fileDialog/hcFileDialogView.h"
#include "hc/editor/views/fileDialog/hcFileDialogRequest.h"
#include "hc/editor/serialization/hcFileFormats.h"

namespace hc::editor
{
  static constexpr const char* SAVE_PROJECT_DIALOG_KEY = "SaveProject";
  static constexpr const char* SAVE_PROJECT_AS_DIALOG_KEY = "SaveProjectAs";

  SaveProjectMenuItem::SaveProjectMenuItem(
    ProjectManager& projectManager,
    FileDialogView& fileDialogView
  ) :
    m_projectManager(projectManager),
    m_fileDialogView(fileDialogView)
  {
  }

  void SaveProjectMenuItem::draw()
  {
    if (ImGui::MenuItem("Save Project"))
    {
      if (m_projectManager.isProjectOpen())
      {
        if (m_projectManager.getCurrentProjectPath().empty())
        {
          LogService::Error(
            "Current project does not have a file path. Please use 'Save Project As' to specify a save location."
          );
          
        }
        else
        {
          m_projectManager.saveProject(
            m_projectManager.getCurrentProjectPath()
          );
        }
      }
      else
      {
        IGFD::FileDialogConfig config;
        config.path = ".";

        FileDialogRequest request(
          SAVE_PROJECT_DIALOG_KEY,
          "Save Project",
          serialization::fileFormats::Project::FILE_EXTENSION,
          this,
          config
        );

        m_fileDialogView.openFileDialog(request);
      }
    }

    if (ImGui::MenuItem("Save Project As"))
    {
      IGFD::FileDialogConfig config;
      config.path = ".";

      FileDialogRequest request(
        SAVE_PROJECT_AS_DIALOG_KEY,
        "Save Project As",
        serialization::fileFormats::Project::FILE_EXTENSION,
        this,
        config
      );

      m_fileDialogView.openFileDialog(request);
    }
  }
  void SaveProjectMenuItem::onFileSelected(
    const String& dialogKey,
    const Path& filePathName,
    const Path& currentPath
  )
  {
    if (dialogKey == SAVE_PROJECT_DIALOG_KEY)
      m_projectManager.saveProject(filePathName);
    else if (dialogKey == SAVE_PROJECT_AS_DIALOG_KEY)
    {
      if (m_projectManager.saveProject(filePathName))
        m_projectManager.openProject(filePathName);
    }
  }
}
