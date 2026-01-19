#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class DirectoryReference;

  /**
   * @brief Represents a reference to a file within the editor.
   */
  class FileReference
  {
  public:
    FileReference();
    ~FileReference();

    /**
     * @brief Initializes the FileReference with the given file path and parent
     * directory.
     *
     * @param filePath The path to the file.
     * @param parent Pointer to the parent DirectoryReference (optional).
     * 
     * @return true if the file path is valid and refers to a file, false
     * otherwise.
     */
    bool initialize(const Path& filePath, DirectoryReference* parent = nullptr);

    /**
     * @brief Gets the file name without extension.
     * 
     * @return Reference to the file name string.
     */
    const String& getName() const;

    /**
     * @brief Gets the file name with extension.
     * 
     * @return Reference to the file name with extension string.
     */
    const String& getNameWithExtension() const;

    /**
     * @brief Gets the file extension (including the dot).
     * 
     * @return Reference to the file extension string.
     */
    const String& getExtension() const;

    /**
     * @brief Gets the full file path.
     * 
     * @return Reference to the file path.
     */
    const Path& getFullPath() const;

    /**
     * @brief Gets the parent directory reference.
     *
     * @return Pointer to the parent DirectoryReference. nullptr if no parent.
     */
    DirectoryReference* getParent() const;

    /**
     * @brief Clears the FileReference data.
     */
    void clear();

  private:
    DirectoryReference* m_parent;
    String m_name;
    String m_extension;
    String m_nameWithExtension;
    Path m_fullPath;
  };
}
