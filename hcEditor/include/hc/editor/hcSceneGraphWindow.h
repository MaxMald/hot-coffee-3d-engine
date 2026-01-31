#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  class GameObjectSelectionService;

  /**
   * @brief Scene Graph window view for the Hot Coffee Editor.
   */
  class SceneGraphWindow :
    public AWindowView,
    public IDependencyResolvable
  {
  public:
    SceneGraphWindow();
    virtual ~SceneGraphWindow();

    void resolveDependencies(DependencyContainer& container) override;

  protected:
    SharedPtr<GameObjectSelectionService> m_selectionService;
    SceneManager* m_sceneManager;

    void onDraw() override;
    void drawCreateLayerSection(SceneGraph& sceneGraph);
    void drawSceneGraph(const SceneGraph& sceneGraph);
    void drawGameObjectNode(GameObject* gameObject);
  };
}
