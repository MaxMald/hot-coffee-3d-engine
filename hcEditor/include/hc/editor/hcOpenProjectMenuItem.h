#pragma once

#include "hc/editor/hcIMenuItem.h"
#include "hc/editor/hcIFileDialogCallback.h"

namespace hc::editor
{
  class ProjectManager;
  class FileDialogView;

  /**
   * @brief Menu item that opens a project selection dialog.
   */
  class OpenProjectMenuItem :
    public IMenuItem,
    public IFileDialogCallback
  {
  public:
    /**
     * @brief Constructs an OpenProjectMenuItem with the given project manager.
     *
     * @param projectManager Reference to the ProjectManager used to open
     * projects.
     * @param fileDialogView Reference to the FileDialogView for displaying file
     * dialogs.
     */
    OpenProjectMenuItem(ProjectManager& projectManager, FileDialogView& fileDialogView);
    ~OpenProjectMenuItem() override = default;

    /**
     * @brief Draws the menu item and handles the open project dialog.
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
