#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcAWindowView.h"

namespace hc
{
  class SceneManager;
  class GameObject;
}

namespace hc::editor
{
  class SceneGraphWindow : public AWindowView
  {
  public:
    SceneGraphWindow();
    virtual ~SceneGraphWindow();

    void setSceneManager(SceneManager* sceneManager);

  protected:
    virtual void onDraw() override;

  private:
    SceneManager* m_sceneManager;

    void drawGameObjectNode(GameObject* gameObject);
  };
}
