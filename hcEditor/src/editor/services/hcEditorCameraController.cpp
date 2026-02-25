#include "hc/editor/services/hcEditorCameraController.h"

namespace hc::editor
{
  EditorCameraController::EditorCameraController(
    SceneManager& sceneManager, InputManager& inputManager
  ) :
    m_sceneManager(sceneManager),
    m_inputManager(inputManager),
    m_cameraMoveScale(0.005f),
    m_cameraZoomScale(0.1f),
    m_cameraRollScale(0.01f),
    m_minDistanceToTarget(0.01f),
    m_target(0.0f, 0.0f, 0.0f)
  {
  }

  void EditorCameraController::update(const Time&)
  {
    if (isMouseMiddleButtonPressed())
    {
      if (isShiftKeyPressed())
        localMove();
      else if (isAltKeyPressed())
        roll();
      else
        orbit();
    }
    else if (isScrollingVertically())
    {
      zoom();
    } 
  }

  void EditorCameraController::destroy()
  {
    // TODO
  }

  bool EditorCameraController::isMouseMiddleButtonPressed() const
  {
    return m_inputManager.isMouseButtonPressed(mouseButtonKey::Middle);
  }

  bool EditorCameraController::isShiftKeyPressed() const
  {
    return m_inputManager.isKeyboardKeyPressed(keyboardKey::LShift) ||
      m_inputManager.isKeyboardKeyPressed(keyboardKey::RShift);
  }

  bool EditorCameraController::isAltKeyPressed() const
  {
    return m_inputManager.isKeyboardKeyPressed(keyboardKey::LAlt) ||
      m_inputManager.isKeyboardKeyPressed(keyboardKey::RAlt);
  }

  bool EditorCameraController::isScrollingVertically() const
  {
    float scrollDelta = m_inputManager
      .getMouseState()
      .getScrollState()
      .getVerticalScrollDelta();

    return scrollDelta != 0.0f;
  }

  void EditorCameraController::localMove()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0 && mouseDelta.y == 0)
      return;       

    Camera& activeCamera = getActiveCamera();

    float distanceToTarget = getCameraDistanceToTarget(activeCamera);
    float distanceModifier = Math::clamp(distanceToTarget, 0.01f, 1.0f);

    float deltaX = -mouseDelta.x * m_cameraMoveScale *  distanceModifier;
    float deltaY = mouseDelta.y * m_cameraMoveScale * distanceModifier;

    Vector3f worldMovement = 
      activeCamera.getRight() * deltaX + 
      activeCamera.getUp() * deltaY;

    m_target += worldMovement;
    activeCamera.move(worldMovement);
  }

  void EditorCameraController::zoom()
  {
    float scrollDelta = m_inputManager
      .getMouseState()
      .getScrollState()
      .getVerticalScrollDelta();

    Camera& activeCamera = getActiveCamera();

    float currentDistance = getCameraDistanceToTarget(activeCamera);
    float deltaZ = scrollDelta * m_cameraZoomScale;
    float maxAllowedDelta = currentDistance - m_minDistanceToTarget;

    if (deltaZ > maxAllowedDelta)
      deltaZ = maxAllowedDelta;

    Vector3f desiredPosition = activeCamera.getPosition() +
      activeCamera.getDirection() * deltaZ;

    activeCamera.setPosition(desiredPosition);
  }

  void EditorCameraController::orbit()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0 && mouseDelta.y == 0)
      return;

    Camera& activeCamera = getActiveCamera();
        
    float yaw = -mouseDelta.x * m_cameraMoveScale;
    float pitch = -mouseDelta.y * m_cameraMoveScale;

    Matrix4 yawRotation = Matrix4::RotationAxis(activeCamera.getUp(), yaw);
    Matrix4 pitchRotation = Matrix4::RotationAxis(activeCamera.getRight(), pitch);
    Matrix4 combinedRotation = yawRotation * pitchRotation;

    Vector3f targetToCamera = activeCamera.getPosition() - m_target;
    Vector4f rotatedVector = (combinedRotation * Vector4f(targetToCamera, 1.0f));

    activeCamera.setPosition(m_target + rotatedVector.xyz());
    activeCamera.lookAt(m_target);
  }

  void EditorCameraController::roll()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0 && mouseDelta.y == 0)
      return;

    Angle rollAmount = Angle::FromRadians(-mouseDelta.x * m_cameraRollScale);
    getActiveCamera().roll(rollAmount);
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

  float EditorCameraController::getCameraDistanceToTarget(const Camera& camera) const
  {
    return (camera.getPosition() - m_target).length();
  }
}
