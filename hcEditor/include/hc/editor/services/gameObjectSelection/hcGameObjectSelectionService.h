#pragma once

#include "hc/editor/services/hcIEditorService.h"
#include "hc/editor/services/editorSceneManager/hcIEditorSceneManagerListener.h"

namespace hc::editor
{
  class IGameObjectSelectionServiceListener;
  class EditorSceneManager;

  /**
   * @brief Manages the selection state of GameObjects in the editor.
   */
  class GameObjectSelectionService :
    public IEditorService,
    public IEditorSceneManagerListener
  {
  public:
    GameObjectSelectionService(EditorSceneManager& editorSceneManager);
    virtual ~GameObjectSelectionService() = default;

    /**
     * @copydoc IEditorService::prepare
     */
    void prepare() override;

    /**
     * @copydoc IEditorService::destroy
     */
    void destroy() override;

    /**
     * @copydoc IEditorSceneManagerListener::onSceneOpened
     */
    void onSceneOpened() override;

    /**
     * @copydoc IEditorSceneManagerListener::onSceneClosed
     */
    void onSceneClosed() override;

    /**
     * @copydoc IEditorSceneManagerListener::onSceneCleared
     */
    void onSceneCleared() override;

    /**
     * @brief Returns the first selected GameObject, or nullptr if none are
     * selected.
     * 
     * @return Pointer to the first selected GameObject, or nullptr if no
     * GameObjects are selected.
     */
    GameObject* getFirstSelectedGameObject() const;

    /**
     * @brief Returns the currently selected GameObjects.
     * 
     * @return A constant reference to the vector of selected GameObjects.
     */
    const Vector<GameObject*>& getSelectedGameObjects() const;

    /**
     * @brief Selects a GameObject. If already selected, does nothing.
     * 
     * @param gameObject The GameObject to select.
     */
    void selectGameObject(GameObject* gameObject);

    /**
     * @brief Deselects a GameObject. If not selected, does nothing.
     * 
     * @param gameObject The GameObject to deselect.
     */
    void deselectGameObject(GameObject* gameObject);

    /**
     * @brief Clears the current selection of GameObjects.
     */
    void clearSelection();

    /**
     * @brief Checks if a GameObject is currently selected.
     * 
     * @param gameObject The GameObject to check.
     * 
     * @return true if the GameObject is selected, false otherwise.
     */
    bool isGameObjectSelected(GameObject* gameObject) const;

    /**
     * @brief Checks if there are any selected GameObjects.
     * 
     * @return true if there are selected GameObjects, false otherwise.
     */
    bool hasSelectedGameObjects() const;

    /**
     * @brief Subscribes a listener to selection change events.
     * 
     * @param listener The listener to subscribe.
     */
    void subscribe(IGameObjectSelectionServiceListener* listener);

    /**
     * @brief Unsubscribes a listener from selection change events.
     * 
     * @param listener The listener to unsubscribe.
     */
    void unsubscribe(IGameObjectSelectionServiceListener* listener);

  private:
    EditorSceneManager& m_editorSceneManager;
    Vector<GameObject*> m_selectedGameObjects;
    Vector<IGameObjectSelectionServiceListener*> m_listeners;
  };
}
