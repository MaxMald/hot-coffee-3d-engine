#include "hc/editor/views/mainMenuBar/hcSaveAsSceneMenuItem.h"

#include <imgui.h>

#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"

namespace hc::editor
{
  SaveAsSceneMenuItem::SaveAsSceneMenuItem(
    ProjectManager& projectManager,
    EditorSceneManager& editorSceneManager,
    ProjectFileDialogView& projectFileDialogView
  ) :
    m_projectManager(projectManager),
    m_editorSceneManager(editorSceneManager),
    m_projectFileDialogView(projectFileDialogView)
  {
  }

  SaveAsSceneMenuItem::~SaveAsSceneMenuItem()
  {
  }

  void SaveAsSceneMenuItem::draw()
  {
    if (ImGui::MenuItem("Save Scene As..."))
    {
      if (!m_projectManager.isProjectOpen())
      {
        LogService::Error("Cannot save scene. No project is currently open.");
        return;
      }

      m_projectFileDialogView.openFileSelector(
        "Save Scene As",
        Vector<String>({ hc::serialization::fileFormat::Scene::FILE_EXTENSION }),
        [this](const Path& selectedPath)
        {
          if (selectedPath.empty())
            return;

          if (!m_editorSceneManager.saveScene(selectedPath))
          {
            LogService::Error(
              String::Format(
                "Failed to save scene as: %s",
                selectedPath.toString().c_str()
              )
            );
            return;
          }
          LogService::Message(
            String::Format(
              "Scene saved as: %s",
              selectedPath.toString().c_str()
            )
          );
        },
        []() {},
        true
      );
    }
  }
}
