#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class GameObjectSelectionService;

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

  protected:
    SceneManager& m_sceneManager;
    GameObjectSelectionService& m_gameObjectSelectionService;

    void onDraw() override;
    void drawCreateLayerSection(Scene& scene);
    void drawSceneGraph(const SceneGraph& sceneGraph);
    void drawGameObjectNode(GameObject* gameObject);
  };
}
