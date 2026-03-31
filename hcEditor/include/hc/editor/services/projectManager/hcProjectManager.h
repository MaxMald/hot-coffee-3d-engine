#pragma once

#include "hc/editor/services/hcIEditorService.h"
#include "hc/editor/services/projectManager/hcProject.h"

namespace hc::editor
{
  class IProjectManagerListener;

  /**
   * @brief Manages the lifecycle and state of editor projects.
   */
  class ProjectManager : public IEditorService
  {
  public:
    /**
     * @brief Constructs a ProjectManager with the given AssetManager reference.
     *
     * @param assetManager Reference to the AssetManager for loading and saving project
     * assets.
     */
    ProjectManager(IAssetManager& assetManager);
    virtual ~ProjectManager() = default;

    /**
     * @copydoc IEditorService::prepare
     */
    void prepare() override;

    /**
     * @copydoc IEditorService::destroy
     */
    void destroy() override;

    /**
     * @brief Opens a project from the specified path.
     * 
     * @param projectPath The file system path to the project to open.
     * 
     * @return True if the project was successfully opened, false otherwise.
     */
    bool openProject(const Path& projectPath);

    /**
     * @brief Saves the currently open project to the specified path.
     * 
     * @param savePath The file system path to save the project to.
     * 
     * @return True if the project was successfully saved, false otherwise.
     */
    bool saveProject(const Path& savePath);

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
    IAssetManager& m_assetManager;
    bool m_isProjectOpen;
    UniquePtr<Project> m_currentProject;
    Vector<IProjectManagerListener*> m_listeners;
  };
}
