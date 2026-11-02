#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor::projectFileDialog
{
  /**
   * @brief Interface for handling file dialog requests in the project file dialog view.
   */
  class IProjectFileDialogRequest
  {
  public:
    /**
     * @brief Virtual destructor for the interface.
     */
    virtual ~IProjectFileDialogRequest() = default;

    /**
     * @brief Retrieves the title of the file dialog.
     *
     * @return The title of the file dialog as a String reference.
     */
    virtual inline const String& getTitle() const = 0;

    /**
     * @brief Retrieves the list of file extension filters for the file dialog.
     *
     * @return A reference to a vector of Strings representing the file extension filters.
     */
    virtual inline const Vector<String>& getFilters() const = 0;

    /**
     * @brief Called when the user selects a file.
     * 
     * @param filePath The path of the selected file.
     */
    virtual void onFileSelected(const Path& filePath) = 0;

    /**
     * @brief Called when the user cancels the file or directory selection.
     */
    virtual void onCancel() = 0;

    /**
     * @brief Destroys the request and cleans up any associated resources.
     */
    virtual void destroy() = 0;

  protected:

    /**
     * @brief Protected constructor to prevent direct instantiation of the interface.
     */
    IProjectFileDialogRequest() = default;
  };
}
