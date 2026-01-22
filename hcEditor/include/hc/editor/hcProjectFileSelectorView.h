#pragma once

#include <hc/hcIDependencyResolvable.h>
#include "hc/editor/hcIView.h"
#include "hc/editor/hcDirectoryNavigator.h"
#include "hc/editor/hcIProjectManagerListener.h"

namespace hc::editor
{
  class ProjectManager;
  class DirectoryReference;
  class FileReference;

  /**
   * @brief View for selecting files or directories within a project.
   */
  class ProjectFileSelectorView :
    public IView,
    public IDependencyResolvable,
    public IProjectManagerListener
  {
  public:
    ProjectFileSelectorView();
    ~ProjectFileSelectorView();

    /**
     * @copydoc IView::draw
     */
    void draw() override;

    /**
     * @copydoc IDependencyResolvable::resolveDependencies
     */
    void resolveDependencies(DependencyContainer& container) override;

    /**
     * @copydoc IProjectManagerListener::onProjectOpened
     */
    void onProjectOpened() override;

    /**
     * @copydoc IProjectManagerListener::onProjectClosed
     */
    void onProjectClosed() override;

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
    SharedPtr<ProjectManager> m_projectManager;
    DirectoryNavigator m_directoryNavigator;
    std::function<void(const Path&)> m_selectionCallback;
    String m_currentTitle;
    Vector<String> m_fileFilters;
    bool m_isFileSelectorOpen;
    bool m_isDirectorySelectorOpen;

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
