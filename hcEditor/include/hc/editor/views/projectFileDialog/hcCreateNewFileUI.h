#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class ProjectFileDialogView;

  /**
   * UI component for creating new files with extension selection.
   *
   * Provides an interface for users to specify a file name, choose from
   * available file type filters, and create a new file in a target
   * directory. Includes validation and error handling for file creation.
   */
  class CreateNewFileUI
  {
  public:
    /**
     * Constructs a CreateNewFileUI instance.
     */
    CreateNewFileUI();

    /**
     * Destroys the CreateNewFileUI instance.
     */
    ~CreateNewFileUI();

    /**
     * Checks if the UI has been initialized with necessary parameters.
     *
     * @return True if initialized, false otherwise.
     */
    bool isInitialized() const;

    /**
     * Initializes the UI with target directory, file type filters, and
     * callback.
     *
     * @param targetDirectory The directory where the new file will be
     *                        created.
     * @param filters A list of file extensions (e.g., ".txt", ".cpp") to
     *                choose from.
     * @param onFileCreated Callback function invoked with the created
     *                      file's path upon success.
     */
    void initialize(
      const Path& targetDirectory,
      const Vector<String>& filters,
      const std::function<void(const Path&)>& onFileCreated
    );

    /**
     * Sets the target directory for file creation.
     *
     * @param targetDirectory The directory where the new file will be created.
     */
    void setTargetDirectory(const Path& targetDirectory);

    /**
     * Renders the UI components for file creation.
     *
     * Displays input fields for file name, a combo box for file type
     * selection, a create button, and error popups when necessary.
     */
    void draw();

    /**
     * Checks if a file was successfully created through this UI.
     *
     * @return True if a file was created, false otherwise.
     */
    bool wasFileCreated() const;

    /**
     * Resets the UI state, clearing all inputs and callbacks. Should be
     * called when the file creation process is completed or cancelled to
     * prepare the UI for the next use.
     */
    void reset();

  private:
    Path m_targetDirectory;
    Vector<String> m_fileFilters;
    std::function<void(const Path&)> m_onFileCreated;
    String m_newFileName;
    Int32 m_selectedFilterIndex;
    String m_popupErrorMessage;
    Bool m_fileCreated;

    void drawFileNameInput();
    void drawFileTypeComboBox();
    void drawCreateButton();
    void drawPopups();
  };
}
