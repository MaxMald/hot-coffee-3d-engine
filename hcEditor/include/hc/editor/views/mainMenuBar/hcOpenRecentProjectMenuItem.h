#pragma once

#include "hc/editor/views/mainMenuBar/hcIMenuItem.h"

namespace hc::editor
{
  class ProjectManager;
  class EditorMetadataManager;

  class OpenRecentProjectMenuItem : public IMenuItem
  {
  public:
    OpenRecentProjectMenuItem(
      ProjectManager& projectManager,
      EditorMetadataManager& editorMetadataManager
    );
    virtual ~OpenRecentProjectMenuItem() override;

    /**
     * @copydoc IMenuItem::draw()
     */
    void draw() override;

  private:
    ProjectManager& m_projectManager;
    EditorMetadataManager& m_editorMetadataManager;
  };
}
