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
    if (receivedLocalMoveCommand())
      localMove();
    else if (receivedOrbitCommand())
      orbit();
    else if (receivedDollyCommand())
      dolly();
  }

  void EditorCameraController::destroy()
  {
    // TODO
  }

  bool EditorCameraController::receivedLocalMoveCommand()
  {
    return ((m_inputManager.isKeyboardKeyPressed(keyboardKey::LShift) ||
      m_inputManager.isKeyboardKeyPressed(keyboardKey::RShift)) &&
      m_inputManager.isMouseButtonPressed(mouseButtonKey::Middle));
  }

  void EditorCameraController::localMove()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0 && mouseDelta.y == 0)
      return;

    Camera& activeCamera = getActiveCamera();
    
    float deltaX = -mouseDelta.x * m_cameraMoveScale;
    float deltaY = mouseDelta.y * m_cameraMoveScale;

    Vector3f worldMovement = 
      activeCamera.getRight() * deltaX + 
      activeCamera.getUp() * deltaY;    
    activeCamera.move(worldMovement);
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

    Camera& activeCamera = getActiveCamera();

    float deltaZ = scrollDelta * m_cameraDollyScale;

    Vector3f worldMovement = activeCamera.getDirection() * deltaZ;
    activeCamera.move(worldMovement);
  }

  bool EditorCameraController::receivedOrbitCommand()
  {
    return m_inputManager.isMouseButtonPressed(mouseButtonKey::Middle);
  }

  void EditorCameraController::orbit()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0 && mouseDelta.y == 0)
      return;

    Vector3f rotation(
      static_cast<float>(-mouseDelta.y * m_cameraMoveScale),
      static_cast<float>(-mouseDelta.x * m_cameraMoveScale),
      0.0f
    );

    Camera& activeCamera = getActiveCamera();
    Vector3f target = activeCamera.getPosition() + activeCamera.getDirection() * 5.0f;
    Vector3f toCamera = activeCamera.getPosition() - target;

    Matrix4 rotationMatrix = Matrix4::Rotation(rotation);
    Vector3f rotatedToCamera = (rotationMatrix * Vector4f(toCamera, 1.0f)).xyz();
    activeCamera.setPosition(target + rotatedToCamera);
    activeCamera.lookAt(target);
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
