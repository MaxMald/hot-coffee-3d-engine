#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc
{
  class GameObject;
}

namespace hc::editor
{
  class GameObjectSelectionService;

  /**
   * @brief Interface for listening to game object selection events in the editor.
   */
  class IGameObjectSelectionServiceListener
  {
  public:
    virtual ~IGameObjectSelectionServiceListener() = default;

  protected:
    /**
     * @brief Called when a game object is selected in the editor.
     *
     * @param gameObject Pointer to the selected GameObject instance.
     */
    virtual void onGameObjectSelected(GameObject* gameObject) = 0;

    /**
     * @brief Called when a game object is deselected in the editor.
     * 
     * @param gameObject Pointer to the deselected GameObject instance.
     */
    virtual void onGameObjectDeselected(GameObject* gameObject) = 0;

    friend class GameObjectSelectionService;
  };
}
