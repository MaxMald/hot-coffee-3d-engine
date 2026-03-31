#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class ProjectManager;

  /**
   * @brief Represents an editor project, providing load and save functionality.
   */
  class Project : public ISerializable
  {
  public:

    /**
     * @brief Creates a new empty Project instance with default version information.
     *
     * @return A pointer to the newly created Project instance. The caller is responsible
     * for managing the memory of the returned Project object.
     */
    static Project* CreateEmpty();
    
    /**
     * @brief Constructs a new Project instance.
     */
    Project();

    /**
     * @brief Destroys the Project instance.
     */
    ~Project();

    /**
     * @brief Serializes the project data to a binary writer.
     *
     * @param writer The binary writer to serialize to.
     */
    void serialize(BinaryWriter& writer) const override;

    /**
     * @brief Deserializes the project data from a binary reader.
     *
     * @param reader The binary reader to deserialize from.
     */
    void deserialize(BinaryReader& reader) override;

    /**
     * @brief Gets the absolute path to the project file.
     * 
     * @return The path to the project file.
     */
    const Path& getProjectFilePath() const;

    /**
     * @brief Sets the path to the last opened scene in the project.
     * 
     * @param absolutePath The absolute path to the scene file.
     */
    void setPathToLastOpenedScene(const Path& absolutePath);

    /**
     * @brief Gets the relative path to the last opened scene in the project.
     *
     * @return The relative path to the last opened scene, or an empty string if no
     * scene has been opened.
     */
    const String& getPathToLastOpenedScene() const;

  private:
    UInt16 m_majorVersion;
    UInt16 m_minorVersion;
    UInt16 m_patchVersion;
    Path m_projectFilePath;
    String m_relativePathToLastOpenedScene;

    /**
     * @brief Sets the absolute path to the project file.
     * 
     * @param path The absolute path to the project file.
     */
    void setProjectFilePath(const Path& path);

    friend class ProjectManager;
  };
}
