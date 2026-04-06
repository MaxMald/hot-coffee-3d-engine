#include "hc/editor/views/windows/sceneViewport/hcSceneViewportGizmoController.h"

namespace hc::editor
{
  SceneViewportGizmoController::SceneViewportGizmoController() :
    m_activeGameObject(nullptr)
  {
  }

  SceneViewportGizmoController::~SceneViewportGizmoController()
  {
  }

  void SceneViewportGizmoController::update(const Time& elapsedTime)
  {
    // TODO
  }

  void SceneViewportGizmoController::draw()
  {
    // TODO
  }

  void SceneViewportGizmoController::setActiveGameObject(GameObject* gameObject)
  {
    m_activeGameObject = gameObject;
  }

  bool SceneViewportGizmoController::hasActiveGameObject() const
  {
    return m_activeGameObject != nullptr;
  }

  GameObject* SceneViewportGizmoController::getActiveGameObject() const
  {
    return m_activeGameObject;
  }

  void SceneViewportGizmoController::clearActiveGameObject()
  {
    m_activeGameObject = nullptr;
  }
}
