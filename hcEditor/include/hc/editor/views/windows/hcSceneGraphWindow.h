#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class GameObjectSelectionService;

  /**
   * @brief Represents the state of the popup menu for a GameObject in the Scene Graph.
   */
  struct GameObjectPopupMenuState
  {
    bool clickedDelete = false; ///< Indicates if the "Delete" option was clicked in the popup menu.

    void clear()
    {
      clickedDelete = false;
    }
  };

  /**
   * @brief Scene Graph window view for the Hot Coffee Editor.
   */
  class SceneGraphWindow : public AWindowView
  {
  public:
    SceneGraphWindow(
      SceneManager& sceneManager,
      GameObjectSelectionService& gameObjectSelectionService
    );
    virtual ~SceneGraphWindow();

    void destroy() override;

  protected:
    SceneManager& m_sceneManager;
    GameObjectSelectionService& m_gameObjectSelectionService;
    Vector<GameObject*> m_rootChildren;
    GameObjectPopupMenuState m_gameObjectPopupMenuState;

    void onDraw() override;
    void drawCreateLayerSection(Scene& scene);
    void drawSceneGraph(const SceneGraph& sceneGraph);
    void drawGameObjectNode(GameObject* gameObject);
    void drawGameObjectPopupMenu(GameObject* gameObject);
  };
}
