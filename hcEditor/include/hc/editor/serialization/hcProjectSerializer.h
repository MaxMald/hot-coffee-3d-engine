#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class Project;
}

namespace hc::editor::serialization
{
  /**
   * @brief Handles serialization and deserialization of Project instances.
   */
  struct ProjectSerializer
  {
  public:
    /**
     * @brief Deserializes a project from a file.
     * 
     * @param filePath The absolute path to the project file to load.
     * 
     * @return A unique pointer to the deserialized Project instance,
     *         or nullptr if deserialization fails.
     */
    UniquePtr<Project> Deserialize(const Path& filePath);

    /**
     * @brief Serializes a project to a file.
     * 
     * @param project The project instance to serialize.
     * @param filePath The absolute path where the project file will be saved.
     * 
     * @return True if serialization was successful, false otherwise.
     */
    bool Serialize(const Project& project, const Path& filePath);
  };
}
