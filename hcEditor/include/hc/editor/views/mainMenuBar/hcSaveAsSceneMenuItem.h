#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/views/mainMenuBar/hcIMenuItem.h"

namespace hc::editor
{
  class ProjectManager;
  class EditorSceneManager;
  class ProjectFileDialogView;

  class SaveAsSceneMenuItem : public IMenuItem
  {
  public:
    SaveAsSceneMenuItem(
      ProjectManager& projectManager,
      EditorSceneManager& editorSceneManager,
      ProjectFileDialogView& projectFileDialogView
    );

    ~SaveAsSceneMenuItem() override;

    /**
     * @copydoc IMenuItem::draw()
     */
    void draw() override;

  private:
    ProjectManager& m_projectManager;
    EditorSceneManager& m_editorSceneManager;
    ProjectFileDialogView& m_projectFileDialogView;
  };
}
