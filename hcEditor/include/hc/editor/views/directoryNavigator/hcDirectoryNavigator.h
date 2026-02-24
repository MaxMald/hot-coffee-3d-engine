#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class DirectoryReference;

  /**
   * @brief Provides navigation functionality for directories within the editor.
   *
   * Allows initialization with a root directory, navigation to parent and
   * subdirectories, and access to the current directory. Manages the lifetime of
   * the root directory reference.
   */
  class DirectoryNavigator
  {
  public:
    DirectoryNavigator();
    ~DirectoryNavigator();

    /**
     * @brief Initializes the navigator with the given root directory path.
     *
     * Sets the root directory and current directory. If initialization fails,
     * both pointers are set to nullptr.
     *
     * @param rootDirectory The path to the root directory.
     */
    void initialize(const Path& rootDirectory);

    /**
     * @brief Gets the current directory reference.
     *
     * @return Pointer to the current DirectoryReference, or nullptr if not set.
     */
    DirectoryReference* getCurrentDirectory() const;

    /**
     * @brief Navigates to the parent directory of the current directory.
     *
     * If the current directory has a parent, sets the current directory to its parent.
     */
    void navigateToParentDirectory();

    /**
     * @brief Navigates to a subdirectory by name.
     *
     * Searches the current directory's subdirectories for a match by name.
     * If found, sets the current directory to the matched subdirectory.
     *
     * @param subDirectoryName The name of the subdirectory to navigate to.
     * 
     * @return true if navigation was successful, false otherwise.
     */
    bool navigateToSubDirectory(const String& subDirectoryName);

    /**
     * @brief Clears the navigator, releasing the root directory and resetting
     * the current directory.
     */
    void clear();

  private:
    DirectoryReference* m_currentDirectory;
    UniquePtr<DirectoryReference> m_rootDirectory;
  };
}
