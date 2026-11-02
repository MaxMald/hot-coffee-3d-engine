#pragma once

#include "hc/editor/views/hcIView.h"
#include "hc/editor/views/directoryNavigator/hcDirectoryNavigator.h"
#include "hc/editor/views/projectFileDialog/hcCreateNewFileUI.h"
#include "hc/editor/views/projectFileDialog/hcIProjectFileDialogRequest.h"
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
     * @param onCancel Callback invoked when the selection is canceled.
     */
    void openImageFile(
      const std::function<void(const Path&)>& onFileSelected,
      const std::function<void()>& onCancel = []() {}
    );

    /**
     * @brief Opens the image file selector dialog with a context.
     * 
     * @param onFileSelected Callback invoked when an image file is selected.
     * @param destroyContext Callback invoked to destroy the context.
     * @param context User-defined context.
     */
    void openImageFile(
      const std::function<void(const Path&, void*)>& onFileSelected,
      const std::function<void(void*)>& onCancel,
      const std::function<void(void*)>& destroyContext,
      void* context
    );

    /**
     * @brief Opens the model file selector dialog.
     *
     * @param onFileSelected Callback invoked when a model file is selected.
     * @param onCancel Callback invoked when the selection is canceled.
     */
    void openModelFile(
      const std::function<void(const Path&)>& onFileSelected,
      const  std::function<void()>& onCancel = []() {}
    );

    /**
     * @brief Opens the model file selector dialog with a user-defiend context.
     *
     * @param title Dialog title.
     * @param filters List of file extension filters.
     * @param onFileSelected Callback invoked when a model file is selected.
     * @param onCancel Callback invoked when the selection is canceled.
     * @param destroyContext Callback invoked to destroy the context.
     * @param context User-defined context.
     * @param allowCreateNewFile If true, allows the user to create a new file in the
     * dialog. The first filter in the filters list will be used as the default extension
     * for the new file. If filter list is empty, this feature will be disabled regardless
     * of the value of this parameter.
     */
    void openFileSelector(
      const String& title,
      const Vector<String> filters,
      const std::function<void(const Path&, void*)>& onFileSelected,
      const std::function<void(void*)>& onCancel,
      const std::function<void(void*)>& destroyContext,
      void* context,
      bool allowCreateNewFile = false
    );

    /**
     * @brief Opens the file selector dialog.
     *
     * @param title Dialog title.
     * @param filters List of file extension filters.
     * @param onFileSelected Callback invoked when a file is selected.
     * @param onCancel Callback invoked when the selection is canceled.
     * @param allowCreateNewFile If true, allows the user to create a new file in the
     * dialog. The first filter in the filters list will be used as the default extension
     * for the new file. If filter list is empty, this feature will be disabled regardless
     * of the value of this parameter.
     */
    void openFileSelector(
      const String& title,
      const Vector<String>& filters,
      const std::function<void(const Path&)>& onFileSelected,
      const std::function<void()>& onCancel = []() {},
      bool allowCreateNewFile = false
    );

    /**
     * @brief Opens the directory selector dialog.
     *
     * @param title Dialog title.
     * @param onDirectorySelected Callback invoked when a directory is selected.
     * @param onCancel Callback invoked when the selection is canceled.
     */
    void openDirectorySelector(
      const String& title,
      const std::function<void(const Path&)>& onDirectorySelected,
      const std::function<void()>& onCancel = []() {}
    );

  private:
    ProjectManager& m_projectManager;
    UniquePtr<projectFileDialog::IProjectFileDialogRequest> m_currentRequest;
    DirectoryNavigator m_directoryNavigator;
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
    void cancel();
    void logWarningIfAlreadyOpen();
    bool isValidFile(const FileReference& file) const;
    bool onDirectorySelected(const DirectoryReference& directory);
    bool onFileSelected(const FileReference& file);
  };
}
