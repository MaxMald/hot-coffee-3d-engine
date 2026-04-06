#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class SceneViewportGizmoController
  {
  public:
    SceneViewportGizmoController();
    ~SceneViewportGizmoController();

    void update(const Time& elapsedTime);
    void draw();
    void setActiveGameObject(GameObject* gameObject);
    bool hasActiveGameObject() const;
    GameObject* getActiveGameObject() const;
    void clearActiveGameObject();

  private:
    GameObject* m_activeGameObject;
  };
}
