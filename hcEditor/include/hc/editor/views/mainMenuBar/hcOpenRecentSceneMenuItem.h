#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/views/mainMenuBar/hcIMenuItem.h"

namespace hc::editor
{
  class ProjectManager;
  class EditorSceneManager;
  class EditorMetadataManager;

  class OpenRecentSceneMenuItem : public IMenuItem
  {
  public:
    OpenRecentSceneMenuItem(
      ProjectManager& projectManager,
      EditorSceneManager& editorSceneManager,
      EditorMetadataManager& editorMetadataManager
    );
    virtual ~OpenRecentSceneMenuItem() override;

    /**
     * @copydoc IMenuItem::draw()
     */
    void draw() override;

  private:
    ProjectManager& m_projectManager;
    EditorSceneManager& m_editorSceneManager;
    EditorMetadataManager& m_editorMetadataManager;
  };
}
