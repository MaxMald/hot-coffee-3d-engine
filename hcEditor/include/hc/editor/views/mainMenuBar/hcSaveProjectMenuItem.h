#pragma once

#include "hc/editor/views/mainMenuBar/hcIMenuItem.h"
#include "hc/editor/views/fileDialog/hcIFileDialogCallback.h"

namespace hc::editor
{
  class ProjectManager;
  class FileDialogView;

  class SaveProjectMenuItem :
    public IMenuItem,
    public IFileDialogCallback
  {
  public:
    SaveProjectMenuItem(ProjectManager& projectManager, FileDialogView& fileDialogView);
    ~SaveProjectMenuItem() override = default;

    /**
     * @brief Draws the menu item and handles the save project dialog.
     */
    void draw() override;

  private:
    ProjectManager& m_projectManager;
    FileDialogView& m_fileDialogView;

    void onFileSelected(
      const String& dialogKey,
      const Path& filePathName,
      const Path& currentPath
    ) override;
  };
}
