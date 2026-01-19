#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class FileReference;

  /**
   * @brief Represents a reference to a directory within the editor.
   *
   * Provides access to directory metadata such as name, full path, parent
   * directory, and its contained files and subdirectories. Use initialize() to
   * set up the reference with a valid directory path. The refresh() method
   * updates the lists of files and subdirectories to reflect the current state
   * of the filesystem.
   */
  class DirectoryReference
  {
  public:
    DirectoryReference();
    ~DirectoryReference();

    /**
     * @brief Initializes the DirectoryReference with the given directory path
     * and parent.
     *
     * @param directoryPath The path to the directory.
     * @param parent Pointer to the parent DirectoryReference (optional).
     * 
     * @return true if the directory path is valid and refers to a directory,
     * false otherwise.
     */
    bool initialize(const Path& directoryPath, DirectoryReference* parent = nullptr);

    /**
     * @brief Gets the directory name.
     * 
     * @return Reference to the directory name string.
     */
    const String& getName() const;

    /**
     * @brief Gets the full directory path.
     * 
     * @return Reference to the directory path.
     */
    const Path& getFullPath() const;

    /**
     * @brief Gets the parent directory reference.
     * 
     * @return Pointer to the parent DirectoryReference.
     */
    DirectoryReference* getParent() const;

    /**
     * @brief Refreshes the lists of files and subdirectories.
     *
     * Scans the directory and updates the internal lists to reflect the current
     * state.
     */
    void refresh();

    /**
     * @brief Gets the list of files contained in the directory.
     *
     * @return Reference to the vector of unique pointers to FileReference
     * objects.
     */
    const Vector<UniquePtr<FileReference>>& getFiles() const;

    /**
     * @brief Gets the list of subdirectories contained in the directory.
     *
     * @return Reference to the vector of unique pointers to DirectoryReference
     * objects.
     */
    const Vector<UniquePtr<DirectoryReference>>& getSubDirectories() const;

    /**
     * @brief Clears the lists of files and subdirectories.
     */
    void clear();

  private:
    DirectoryReference* m_parent;
    String m_name;
    Path m_fullPath;
    Vector<UniquePtr<FileReference>> m_files;
    Vector<UniquePtr<DirectoryReference>> m_subDirectories;
  };
}
