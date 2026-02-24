#include "hc/editor/services/hcEditorCameraController.h"

namespace hc::editor
{
  EditorCameraController::EditorCameraController(
    SceneManager& sceneManager, InputManager& inputManager
  ) :
    m_sceneManager(sceneManager),
    m_inputManager(inputManager),
    m_cameraMoveScale(0.005f),
    m_cameraDollyScale(0.1f),
    m_cameraRotationSpeed(90.0f)
  {
  }

  void EditorCameraController::update(const Time&)
  {
    if (receivedMoveCommand())
      move();
    if (receivedDollyCommand())
      dolly();
  }

  void EditorCameraController::destroy()
  {
    // TODO
  }

  bool EditorCameraController::receivedMoveCommand()
  {
    return ((m_inputManager.isKeyboardKeyPressed(keyboardKey::LShift) ||
      m_inputManager.isKeyboardKeyPressed(keyboardKey::RShift)) &&
      m_inputManager.isMouseButtonPressed(mouseButtonKey::Middle));
  }

  void EditorCameraController::move()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0 && mouseDelta.y == 0)
      return;

    Vector4f movementDelta(
      static_cast<float>(-mouseDelta.x * m_cameraMoveScale),
      static_cast<float>(mouseDelta.y * m_cameraMoveScale),
      0.0f,
      0.0f
    );

    Camera& activeCamera = getActiveCamera();
    movementDelta = activeCamera.getViewMatrix() * movementDelta;
    activeCamera.move(movementDelta.xyz());
  }

  bool EditorCameraController::receivedDollyCommand()
  {
    float scrollDelta = m_inputManager
      .getMouseState()
      .getScrollState()
      .getVerticalScrollDelta();

    return scrollDelta != 0.0f;
  }

  void EditorCameraController::dolly()
  {
    float scrollDelta = m_inputManager
      .getMouseState()
      .getScrollState()
      .getVerticalScrollDelta();

    Vector4f movementDelta(
      0.0f,
      0.0f,
      static_cast<float>(-scrollDelta * m_cameraDollyScale),
      0.0f
    );

    Camera& activeCamera = getActiveCamera();
    movementDelta = activeCamera.getViewMatrix() * movementDelta;
    activeCamera.move(movementDelta.xyz());
  }

  Camera& EditorCameraController::getActiveCamera()
  {
    Scene* activeScene = m_sceneManager.getActiveScene();
    if (!activeScene)
      throw RuntimeErrorException("No active scene to control camera in.");

    Camera* activeCamera = activeScene->getCameraManager().getActiveCamera();
    if (!activeCamera)
      throw RuntimeErrorException("Active scene has no active camera to control.");

    return *activeCamera;
  }
}
