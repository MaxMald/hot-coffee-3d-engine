#pragma once

#include "hc/editor/services/hcIEditorService.h"
#include "hc/editor/services/projectManager/hcIProjectManagerListener.h"

namespace hc::editor
{
  class ProjectManager;

  /**
   * @brief Manages the editor's scene, allowing opening, saving, and closing scenes.
   * 
   * Listens to project events to ensure scene management is in sync with the current
   * project state.
   */
  class EditorSceneManager :
    public IEditorService,
    public IProjectManagerListener
  {
  public:
    /**
     * @brief Constructs an EditorSceneManager with the given editor scene and project
     * manager.
     *
     * @param editorScene Pointer to the Scene instance that represents the editor's
     * scene.
     * @param assetManager Reference to the asset manager for managing assets.
     * @param graphicsManager Reference to the graphics manager for rendering.
     * @param projectManager Reference to the ProjectManager for subscribing to project
     * events.
     */
    EditorSceneManager(
      Scene* editorScene,
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager,
      ProjectManager& projectManager
    );
    virtual ~EditorSceneManager() = default;

    /**
     * @brief Prepares the editor scene manager by subscribing to project manager events.
     */
    void prepare() override;

    /**
     * @brief Destroys the editor scene manager by unsubscribing from project manager
     * events.
     */
    void destroy() override;

    /**
     * @brief Opens a scene from the specified path.
     *
     * @param scenePath The file path to the scene to open.
     * @return True if the scene was successfully opened, false otherwise.
     */
    bool openScene(const Path& scenePath);

    /**
     * @brief Saves the current scene to the specified path.
     *
     * @param scenePath The file path where the scene should be saved.
     * @return True if the scene was successfully saved, false otherwise.
     */
    bool saveScene(const Path& scenePath);

    /**
     * @brief Closes the currently open scene.
     */
    void closeScene();

    /**
     * @brief Checks whether a scene is currently open.
     *
     * @return True if a scene is open, false otherwise.
     */
    bool isSceneOpen() const;

    /**
     * @brief Retrieves the path of the currently open scene.
     *
     * @return A reference to the path of the current scene.
     */
    const Path& getCurrentScenePath() const;

  protected:

    /**
     * @brief Called when a project is opened.
     */
    void onProjectOpened() override;

    /**
     * @brief Called when a project is closed.
     */
    void onProjectClosed() override;

  private:
    IAssetManager& m_assetManager;
    IGraphicsManager& m_graphicsManager;
    ProjectManager& m_projectManager;
    Scene* m_editorScene;
    Path m_currentScenePath;

    void updateLastOpenedSceneInProject();
    void assertSceneIsValid() const;
  };
}
