#include "hc/editor/hcEditorCameraController.h"

namespace hc::editor
{
  EditorCameraController::EditorCameraController(SceneManager& sceneManager) :
    m_sceneManager(sceneManager),
    m_cameraMoveSpeed(5.0f),
    m_cameraRotationSpeed(90.0f)
  {
  }

  void EditorCameraController::update(const Time&)
  {
    // TODO
  }

  void EditorCameraController::destroy()
  {
    // TODO
  }
}
