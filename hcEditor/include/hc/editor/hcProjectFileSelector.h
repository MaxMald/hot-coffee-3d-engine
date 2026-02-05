#pragma once

#include "hc/editor/hcABaseView.h"
#include "hc/editor/hcDirectoryNavigator.h"
#include "hc/editor/hcIProjectManagerListener.h"

namespace hc::editor
{
  class DirectoryReference;
  class FileReference;

  /**
   * @brief View for selecting files or directories within a project.
   */
  class ProjectFileSelector :
    public ABaseView,
    public IProjectManagerListener
  {
  public:
    /**
     * @brief Opens an image file selector dialog.
     * 
     * @param onFileSelected Callback invoked when an image file is selected.
     */
    static void OpenImageFile(
      const std::function<void(const Path&)>& onFileSelected
    );

    /**
     * @brief Opens a model file selector dialog.
     * 
     * @param onFileSelected Callback invoked when a model file is selected.
     */
    static void OpenModelFile(
      const std::function<void(const Path&)>& onFileSelected
    );

    /**
     * @brief Opens a file selector dialog with custom title and filters.
     *
     * @param title Dialog title.
     * @param filters List of file extension filters.
     * @param onFileSelected Callback invoked when a file is selected.
     */
    static void OpenFile(
      const String& title,
      const Vector<String>& filters,
      const std::function<void(const Path&)>& onFileSelected
    );

    /**
     * @brief Opens a directory selector dialog with custom title.
     * 
     * @param title Dialog title.
     * @param onDirectorySelected Callback invoked when a directory is selected.
     */
    static void OpenDirectory(
      const String& title,
      const std::function<void(const Path&)>& onDirectorySelected
    );

    ProjectFileSelector();
    ~ProjectFileSelector() override;

    /**
     * @copydoc ABaseView::draw
     */
    void draw() override;

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
