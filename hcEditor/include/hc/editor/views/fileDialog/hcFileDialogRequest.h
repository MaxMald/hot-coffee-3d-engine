#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/views/fileDialog/hcIFileDialogCallback.h"
#include "ImGuiFileDialog.h"

namespace hc::editor
{
  /**
   * @brief Represents a request to open a file dialog in the editor.
   */
  struct FileDialogRequest
  {
    /**
     * @brief Unique key to identify the dialog instance.
     */
    String dialogKey;

    /**
     * @brief Title displayed on the file dialog window.
     */
    String dialogTitle;

    /**
     * @brief Filter for allowed file extensions (e.g., ".txt,.cpp").
     */
    const char* fileExtensionFilter;

    /**
     * @brief Configuration for the ImGuiFileDialog instance.
     */
    IGFD::FileDialogConfig igfdConfiguration;

    /**
     * @brief Callback function invoked when a file is selected.
     */
    IFileDialogCallback* callback;

    /**
     * @brief Default constructor for FileDialogRequest.
     */
    FileDialogRequest() :
      dialogKey(""),
      dialogTitle(""),
      fileExtensionFilter(""),
      igfdConfiguration(),
      callback(nullptr)
    {
    }

    /**
     * @brief Constructs a FileDialogRequest with the specified parameters.
     *
     * @param _dialogKey Unique key for the dialog.
     * @param _dialogTitle Title for the dialog window.
     * @param _fileExtensionFilter Filter for file extensions.
     * @param _callback Optional callback for when a file is selected.
     * @param _igfdConfiguration Optional configuration for the dialog.
     */
    FileDialogRequest(
      const String& _dialogKey,
      const String& _dialogTitle,
      const char* _fileExtensionFilter,
      IFileDialogCallback* _callback = nullptr,
      const IGFD::FileDialogConfig& _igfdConfiguration = IGFD::FileDialogConfig()
    ) :
      dialogKey(_dialogKey),
      dialogTitle(_dialogTitle),
      fileExtensionFilter(_fileExtensionFilter),
      igfdConfiguration(_igfdConfiguration),
      callback(_callback)
    {
    }
  };
}
