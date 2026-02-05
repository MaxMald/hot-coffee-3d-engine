#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  /**
   * @brief Scene Graph window view for the Hot Coffee Editor.
   */
  class SceneGraphWindow : public AWindowView
  {
  public:
    SceneGraphWindow();
    virtual ~SceneGraphWindow();

  protected:
    void onDraw() override;
    void drawCreateLayerSection(SceneGraph& sceneGraph);
    void drawSceneGraph(const SceneGraph& sceneGraph);
    void drawGameObjectNode(GameObject* gameObject);
  };
}
