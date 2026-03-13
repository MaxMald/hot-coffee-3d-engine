#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class FileDialogView;

  /**
   * @brief Interface for receiving callbacks from file dialog interactions in
   * the editor.
   */
  class IFileDialogCallback
  {
  public:
    virtual ~IFileDialogCallback() = default;

  protected:
    IFileDialogCallback() = default;

    /**
     * @brief Callback method invoked when a file is selected in the file dialog.
     *
     * @param dialogKey The unique key identifying the file dialog instance.
     * @param filePathName The full path of the selected file.
     * @param currentPath The current directory path in the dialog.
     */
    virtual void onFileSelected(
      const String& dialogKey,
      const Path& filePathName,
      const Path& currentPath
    ) = 0;

    friend class FileDialogView;
  };
}
