#pragma once

#include "hc/editor/services/hcIEditorService.h"
#include "hc/editor/services/projectManager/hcIProjectManagerListener.h"

namespace hc::editor
{
  class ProjectManager;
  class IEditorSceneManagerListener;
  class EditorMetadataManager;

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
     * @param editorMetadataManager Reference to the EditorMetadataManager.
     */
    EditorSceneManager(
      Scene* editorScene,
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager,
      ProjectManager& projectManager,
      EditorMetadataManager& editorMetadataManager
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
     * @brief Creates a new scene at the specified path.
     * @param scenePath The absolute file path where the new scene should be created.
     * @return True if the scene was successfully created, false otherwise.
     */
    bool createNewScene(const Path& scenePath);

    /**
     * @brief Opens a scene from the specified path.
     * @param scenePath The absolute file path to the scene to open.
     * @return True if the scene was successfully opened, false otherwise.
     */
    bool openScene(const Path& scenePath);

    /**
     * @brief Saves the current scene to the specified path.
     *
     * @param scenePath The absolute file path where the scene should be saved.
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
     * @brief Retrieves the absolute path of the currently open scene.
     *
     * @return A reference to the absolute path of the current scene.
     */
    const Path& getCurrentScenePath() const;

    /**
     * @brief Gets a reference to the editor's scene.
     *
     * @return Reference to the editor's Scene instance.
     *
     * @throw RuntimeErrorException If the editor scene is undefined or invalid.
     */
    Scene& getEditorScene();

    /**
     * @brief Subscribes a listener to editor scene manager events.
     *
     * @param listener Pointer to the IEditorSceneManagerListener to subscribe.
     */
    void subscribeListener(IEditorSceneManagerListener* listener);

    /**
     * @brief Unsubscribes a listener from editor scene manager events.
     *
     * @param listener Pointer to the IEditorSceneManagerListener to unsubscribe.
     */
    void unsubscribeListener(IEditorSceneManagerListener* listener);

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
    EditorMetadataManager& m_editorMetadataManager;
    Scene* m_editorScene;
    Path m_currentScenePath;
    Vector<IEditorSceneManagerListener*> m_listeners;

    /**
     * @brief Adds the last opened scene path to the editor metadata.
     * @param absoluteScenePath The absolute path of the scene to add to metadata.
     */
    void addLastOpenedSceneToMetadata(const Path& absoluteScenePath);

    /**
     * @brief Asserts that the editor scene is valid and throws an exception if not.
     * @throw RuntimeErrorException If the editor scene is undefined or invalid.
     */
    void assertSceneIsValid() const;

    /**
     * @brief Clears the current scene, removing all game objects and resetting the scene
     * state.
     */
    void clearScene();
  };
}
