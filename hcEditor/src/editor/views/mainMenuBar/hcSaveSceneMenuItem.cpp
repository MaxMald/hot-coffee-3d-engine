#include "hc/editor/views/mainMenuBar/hcSaveSceneMenuItem.h"
#include <imgui.h>
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"

namespace hc::editor
{
  SaveSceneMenuItem::SaveSceneMenuItem(
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

  void SaveSceneMenuItem::draw()
  {
    if (ImGui::MenuItem("Save Scene"))
    {
      if (!m_projectManager.isProjectOpen())
      {
        LogService::Error("Cannot save scene: No project is currently open.");
        return;
      }

      if (!m_editorSceneManager.isSceneOpen())
      {
        createAndSaveNewScene();
        return;
      }

      m_editorSceneManager.saveScene(
        m_editorSceneManager.getCurrentScenePath()
      );
    }
  }

  void SaveSceneMenuItem::createAndSaveNewScene()
  {
    m_projectFileDialogView.openFileSelector(
      "Save New Scene",
      m_sceneFileExtensionFilter,
      [this](const Path& savePath)
      {
        if (savePath.empty())
        {
          LogService::Warning("Save scene cancelled: No file path provided.");
          return;
        }

        if (!m_editorSceneManager.saveScene(savePath))
        {
          LogService::Error("Failed to save scene to path: " + savePath.string());
          return;
        }

        LogService::Message("Scene saved successfully to: " + savePath.string());
      },
      true
    );
  }
}
