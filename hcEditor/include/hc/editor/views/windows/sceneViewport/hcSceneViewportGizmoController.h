#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include <ImGuizmo.h>

namespace hc::editor
{
  class SceneViewportGizmoController
  {
  public:
    SceneViewportGizmoController(InputManager& inputManager, Camera& camera);
    ~SceneViewportGizmoController();

    void update(const Time& elapsedTime);
    void draw(const Vector2f& windowPosition, const Vector2f& windowSize);
    void setActiveGameObject(GameObject* gameObject);
    bool hasActiveGameObject() const;
    GameObject* getActiveGameObject() const;
    void clearActiveGameObject();

  private:
    InputManager& m_inputManager;
    GameObject* m_activeGameObject;
    Camera& m_camera;
    ImGuizmo::OPERATION m_currentOperation;
    ImGuizmo::MODE m_currentMode;

    void drawGizmo(const Vector2f& windowPosition, const Vector2f& windowSize);
    void drawCubeView(const Vector2f& windowPosition, const Vector2f& windowSize);
  };
}
