#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/metadata/hcEditorMetadataStructures.h"

namespace hc::editor
{
  /**
   * @brief Manages project metadata for the editor.
   */
  class EditorProjectMetadataManager
  {
  public:
    EditorProjectMetadataManager() = default;
    ~EditorProjectMetadataManager() = default;

    /**
     * @brief Gets the relative paths of the last opened scenes in the current project.
     * @returns A constant reference to a vector of Paths representing the last opened
     * scenes (relative paths).
     */
    inline const Vector<Path>& getLastOpenedScenePaths() const
    {
      return m_projectMetadata.lastOpenedScenes;
    }

    /**
     * @brief Saves the path of the last opened scene in the current project.
     * @param projectFilepath The absolute path to the project file associated with the
     * scene.
     * @param scenePath The absolute path of the scene to save as the last opened scene.
     */
    void saveLastOpenedScenePath(
      const Path& projectFilepath,
      const Path& scenePath
    );

    /**
     * @brief Loads the project metadata from the specified project path.
     * @param projectFilepath The absolute path to the project file from which to load
     * metadata.
     */
    void loadProjectMetadata(const Path& projectFilepath);
    
    /**
     * @brief Saves the project metadata to the specified project path.
     * @param projectFilepath The absolute path to the project file to which to save
     * metadata.
     */
    void saveProjectMetadata(const Path& projectFilepath) const;

    /**
     * @brief Clears the project metadata, resetting it to an empty state.
     */
    void clearProjectMetadata();

  private:
    metadata::ProjectMetadata m_projectMetadata;
  };
}
