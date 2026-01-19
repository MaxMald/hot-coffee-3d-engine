#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcProjectFileContent.h"

namespace hc::editor
{
  /**
   * @brief Represents an editor project, providing load and save functionality.
   */
  class Project
  {
  public:
    Project();
    ~Project();

    /**
     * @brief Loads the project data from a file.
     * 
     * @param filePath The path to the project file to load.
     * 
     * @return True if the project was loaded successfully, false otherwise.
     */
    bool loadFromFile(const Path& filePath);

    /**
     * @brief Saves the project data to a file.
     * 
     * @param filePath The path to the file where the project will be saved.
     * 
     * @return True if the project was saved successfully, false otherwise.
     */
    bool saveToFile(const Path& filePath) const;

    /**
     * @brief Gets a reference to the project file content.
     * 
     * @return Reference to the ProjectFileContent instance.
     */
    ProjectFileContent& getProjectFileContent();

    /**
     * @brief Gets a const reference to the project file content.
     * 
     * @return Const reference to the ProjectFileContent instance.
     */
    const ProjectFileContent& getProjectFileContent() const;

  private:
    ProjectFileContent m_projectFileContent;
  };
}
