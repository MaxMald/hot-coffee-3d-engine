#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc
{
  class GameObject;
}

namespace hc::editor
{
  class IGameObjectSelectionServiceListener;

  /**
   * @brief Manages the selection state of GameObjects in the editor.
   */
  class GameObjectSelectionService
  {
  public:
    GameObjectSelectionService();
    ~GameObjectSelectionService();

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
    Vector<GameObject*> m_selectedGameObjects;
    Vector<IGameObjectSelectionServiceListener*> m_listeners;
  };
}
