#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class Project;
  class IProjectManagerListener;

  /**
   * @brief Manages the lifecycle and state of editor projects.
   */
  class ProjectManager
  {
  public:
    ProjectManager();
    ~ProjectManager();

    /**
     * @brief Opens a project from the specified path.
     * 
     * @param projectPath The file system path to the project to open.
     * 
     * @return True if the project was successfully opened, false otherwise.
     */
    bool openProject(const Path& projectPath);

    /**
     * @brief Closes the currently open project, if any.
     * 
     * @return True if a project was closed, false if no project was open.
     */
    bool closeProject();

    /**
     * @brief Gets the file system path of the currently open project.
     *
     * @return The path to the current project, or an empty path if no project is
     * open.
     */
    Path getCurrentProjectPath() const;

    /**
     * @brief Gets the directory of the currently open project.
     *
     * @return The directory path of the current project, or an empty path if no
     * project is open.
     */
    Path getCurrentProjectDirectory() const;

    /**
     * @brief Checks if a project is currently open.
     * 
     * @return True if a project is open, false otherwise.
     */
    bool isProjectOpen() const;

    /**
     * @brief Gets a pointer to the currently open project.
     *
     * @return A pointer to the current Project, or nullptr if no project is
     * open.
     */
    Project* getCurrentProject();

    /**
    * @brief Subscribes a listener to project manager events.
    */
    void subscribeListener(IProjectManagerListener* listener);

    /**
    * @brief Unsubscribes a listener from project manager events.
    */
    void unsubscribeListener(IProjectManagerListener* listener);

  private:
    Path m_currentProjectPath;
    bool m_isProjectOpen;
    UniquePtr<Project> m_currentProject;
    Vector<IProjectManagerListener*> m_listeners;
  };
}
