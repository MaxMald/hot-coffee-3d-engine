#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/views/mainMenuBar/hcIMenuItem.h"

namespace hc::editor
{
  class ProjectManager;
  class EditorSceneManager;
  class ProjectFileDialogView;

  class CreateSceneMenuItem : public IMenuItem
  {
  public:
    CreateSceneMenuItem(
      ProjectManager& projectManager,
      EditorSceneManager& editorSceneManager,
      ProjectFileDialogView& projectFileDialogView
    );

    virtual ~CreateSceneMenuItem() override;

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
