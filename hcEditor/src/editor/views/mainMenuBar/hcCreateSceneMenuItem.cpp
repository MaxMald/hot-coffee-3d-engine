#include "hc/editor/views/mainMenuBar/hcCreateSceneMenuItem.h"

#include <imgui.h>

#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"

namespace hc::editor
{
  CreateSceneMenuItem::CreateSceneMenuItem(
    ProjectManager& projectManager,
    EditorSceneManager& editorSceneManager,
    ProjectFileDialogView& projectFileDialogView
  ) :
    m_projectManager(projectManager),
    m_editorSceneManager(editorSceneManager),
    m_projectFileDialogView(projectFileDialogView)
  {
  }

  CreateSceneMenuItem::~CreateSceneMenuItem()
  {
  }

  void CreateSceneMenuItem::draw()
  {
    if (ImGui::MenuItem("New Scene"))
    {
      if (!m_projectManager.isProjectOpen())
      {
        LogService::Error("Cannot create a new scene. No project is currently open.");
        return;
      }

      m_projectFileDialogView.openFileSelector(
        "Create New Scene",
        Vector<String>({ hc::serialization::fileFormat::Scene::FILE_EXTENSION }),
        [this](const Path& selectedPath)
        {
          if (selectedPath.empty())
            return;

          if (!m_editorSceneManager.createNewScene(selectedPath))
          {
            LogService::Error(
              String::Format(
                "Failed to create new scene at: %s",
                selectedPath.toString().c_str()
              )
            );
            return;
          }

          LogService::Message(
            String::Format(
              "New scene created at: %s",
              selectedPath.toString().c_str()
            )
          );
        },
        [this]()
        {
          LogService::Message("New scene creation canceled.");
        },
        true
      );
    }
  }
}
