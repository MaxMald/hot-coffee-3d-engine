#include "hc/editor/views/mainMenuBar/hcOpenSceneMenuItem.h"
#include <imgui.h>
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"

namespace hc::editor
{
  OpenSceneMenuItem::OpenSceneMenuItem(
    ProjectManager& projectManager,
    EditorSceneManager& editorSceneManager,
    ProjectFileDialogView& projectFileDialogView
  ) :
    m_projectManager(projectManager),
    m_editorSceneManager(editorSceneManager),
    m_projectFileDialogView(projectFileDialogView),
    m_sceneFileExtensionFilter({ hc::serialization::fileFormat::Scene::FILE_EXTENSION })
  {
  }

  void OpenSceneMenuItem::draw()
  {
    if (ImGui::MenuItem("Open Scene"))
    {
      if (!m_projectManager.isProjectOpen())
      {
        LogService::Error("Cannot open scene: No project is currently open.");
        return;
      }

      m_projectFileDialogView.openFileSelector(
        "Open Scene",
        m_sceneFileExtensionFilter,
        [this](const Path& selectedPath)
        {
          if (selectedPath.empty())
          {
            LogService::Warning("Open scene cancelled: No file path selected.");
            return;
          }

          if (!m_editorSceneManager.openScene(selectedPath))
          {
            LogService::Error("Failed to open scene from path: " + selectedPath.string());
          }
        }
      );
    }
  }
}
