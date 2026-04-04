#pragma once

#include "hc/editor/views/hcIView.h"
#include "hc/editor/views/directoryNavigator/hcDirectoryNavigator.h"
#include "hc/editor/views/projectFileDialog/hcCreateNewFileUI.h"
#include "hc/editor/services/projectManager/hcIProjectManagerListener.h"

namespace hc::editor
{
  class DirectoryReference;
  class FileReference;
  class ProjectManager;

  /**
   * @brief View for selecting files or directories within a project.
   */
  class ProjectFileDialogView :
    public IView,
    public IProjectManagerListener
  {
  public:
    ProjectFileDialogView(ProjectManager& projectManager);
    virtual ~ProjectFileDialogView();

    /**
     * @copydoc IView::update
     */
    void update(const Time& elapsedTime) override;

    /**
     * @copydoc ABaseView::draw
     */
    void draw() override;

    /**
     * @copydoc IView::destroy
     */
    void destroy() override;

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
     * @param allowCreateNewFile If true, allows the user to create a new file in the
     * dialog. The first filter in the filters list will be used as the default extension
     * for the new file. If filter list is empty, this feature will be disabled regardless
     * of the value of this parameter.
     */
    void openFileSelector(
      const String& title,
      const Vector<String>& filters,
      const std::function<void(const Path&)>& onFileSelected,
      bool allowCreateNewFile = false
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
    CreateNewFileUI m_createNewFileUI;

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
