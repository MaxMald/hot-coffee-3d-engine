#pragma once

#include "hc/editor/views/hcABaseView.h"
#include "hc/editor/views/directoryNavigator/hcDirectoryNavigator.h"
#include "hc/editor/services/projectManager/hcIProjectManagerListener.h"

namespace hc::editor
{
  class DirectoryReference;
  class FileReference;
  class ProjectManager;

  /**
   * @brief View for selecting files or directories within a project.
   */
  class ProjectFileSelector :
    public ABaseView,
    public IProjectManagerListener
  {
  public:
    ProjectFileSelector(ProjectManager& projectManager);
    virtual ~ProjectFileSelector();

    /**
     * @copydoc ABaseView::draw
     */
    void draw() override;

    /**
     * @brief Opens the image file selector dialog.
     * 
     * @param onFileSelected Callback invoked when an image file is selected.
     */
    void openImageFile(
      const std::function<void(const Path&)>& onFileSelected
    );

    /**
     * @brief Opens the model file selector dialog.
     *
     * @param onFileSelected Callback invoked when a model file is selected.
     */
    void openModelFile(
      const std::function<void(const Path&)>& onFileSelected
    );

    /**
     * @brief Opens the file selector dialog.
     *
     * @param title Dialog title.
     * @param filters List of file extension filters.
     * @param onFileSelected Callback invoked when a file is selected.
     */
    void openFileSelector(
      const String& title,
      const Vector<String>& filters,
      const std::function<void(const Path&)>& onFileSelected
    );

    /**
     * @brief Opens the directory selector dialog.
     *
     * @param title Dialog title.
     * @param onDirectorySelected Callback invoked when a directory is selected.
     */
    void openDirectorySelector(
      const String& title,
      const std::function<void(const Path&)>& onDirectorySelected
    );

  private:
    ProjectManager& m_projectManager;
    DirectoryNavigator m_directoryNavigator;
    std::function<void(const Path&)> m_selectionCallback;
    String m_currentTitle;
    Vector<String> m_fileFilters;
    Vector<String> m_imageFileExtensions;
    Vector<String> m_modelFileExtensions;
    bool m_isFileSelectorOpen;
    bool m_isDirectorySelectorOpen;

    void onDestroy() override;
    void onProjectOpened() override;
    void onProjectClosed() override;

    void drawDirectorySelectionInterface();
    void drawFileSelectionInterface();
    bool drawBackAndRefreshButtons();
    void clear();
    void logWarningIfAlreadyOpen();
    bool isValidFile(const FileReference& file) const;
    bool onDirectorySelected(const DirectoryReference& directory);
    bool onFileSelected(const FileReference& file);
  };
}
