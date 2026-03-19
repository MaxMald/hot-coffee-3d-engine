#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/views/mainMenuBar/hcIMenuItem.h"

namespace hc::editor
{
  class ProjectManager;
  class EditorSceneManager;
  class ProjectFileDialogView;

  class SaveSceneMenuItem : public IMenuItem
  {
  public:
    SaveSceneMenuItem(
      ProjectManager& projectManager,
      EditorSceneManager& editorSceneManager,
      ProjectFileDialogView& projectFileDialogView
    );

    void draw() override;

  private:
    ProjectManager& m_projectManager;
    EditorSceneManager& m_editorSceneManager;
    ProjectFileDialogView& m_projectFileDialogView;
    Vector<String> m_sceneFileExtensionFilter;

    void createAndSaveNewScene();
  };
}
