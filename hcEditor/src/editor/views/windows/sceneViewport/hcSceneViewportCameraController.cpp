#include "hc/editor/views/windows/sceneViewport/hcSceneViewportCameraController.h"
#include "hc/editor/scenes/hcEditorSceneNames.h"

namespace hc::editor
{
  SceneViewportCameraController::SceneViewportCameraController(
    InputManager& inputManager
  ) :
    m_camera(),
    m_inputManager(inputManager),
    m_cameraMoveScale(0.005f),
    m_cameraZoomScale(0.1f),
    m_cameraRollScale(0.01f),
    m_minDistanceToTarget(0.01f),
    m_target(0.0f, 0.0f, 0.0f)
  {
  }

  Camera& SceneViewportCameraController::getCamera()
  {
    return m_camera;
  }

  void SceneViewportCameraController::prepare()
  {
    m_camera.setPosition(Vector3f(0.0f, 0.0f, 5.0f));
    m_camera.lookAt(Vector3f(0.0f, 0.0f, 0.0f));
  }

  void SceneViewportCameraController::update(const Time&)
  {
    // Update target
    m_target = m_camera.getPosition() + m_camera.getDirection()
      * getCameraDistanceToTarget(m_camera);

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

  bool SceneViewportCameraController::isMouseMiddleButtonPressed() const
  {
    return m_inputManager.isMouseButtonPressed(mouseButtonKey::Middle);
  }

  bool SceneViewportCameraController::isShiftKeyPressed() const
  {
    return m_inputManager.isKeyboardKeyPressed(keyboardKey::LShift) ||
      m_inputManager.isKeyboardKeyPressed(keyboardKey::RShift);
  }

  bool SceneViewportCameraController::isAltKeyPressed() const
  {
    return m_inputManager.isKeyboardKeyPressed(keyboardKey::LAlt) ||
      m_inputManager.isKeyboardKeyPressed(keyboardKey::RAlt);
  }

  bool SceneViewportCameraController::isScrollingVertically() const
  {
    float scrollDelta = m_inputManager
      .getMouseState()
      .getScrollState()
      .getVerticalScrollDelta();

    return scrollDelta != 0.0f;
  }

  void SceneViewportCameraController::localMove()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0 && mouseDelta.y == 0)
      return;

    float distanceToTarget = getCameraDistanceToTarget(m_camera);
    float distanceModifier = Math::Clamp(distanceToTarget, 0.01f, 1.0f);

    float deltaX = -mouseDelta.x * m_cameraMoveScale * distanceModifier;
    float deltaY = mouseDelta.y * m_cameraMoveScale * distanceModifier;

    Vector3f worldMovement =
      m_camera.getRight() * deltaX +
      m_camera.getUp() * deltaY;

    m_target += worldMovement;
    m_camera.move(worldMovement);
  }

  void SceneViewportCameraController::zoom()
  {
    float scrollDelta = m_inputManager
      .getMouseState()
      .getScrollState()
      .getVerticalScrollDelta();

    float currentDistance = getCameraDistanceToTarget(m_camera);
    float deltaZ = scrollDelta * m_cameraZoomScale;
    float maxAllowedDelta = currentDistance - m_minDistanceToTarget;

    if (deltaZ > maxAllowedDelta)
      deltaZ = maxAllowedDelta;

    Vector3f desiredPosition = m_camera.getPosition() +
      m_camera.getDirection() * deltaZ;

    m_camera.setPosition(desiredPosition);
  }

  void SceneViewportCameraController::orbit()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0 && mouseDelta.y == 0)
      return;

    float yaw = -mouseDelta.x * m_cameraMoveScale;
    float pitch = -mouseDelta.y * m_cameraMoveScale;

    Matrix4 yawRotation = Matrix4::RotationAxis(m_camera.getUp(), yaw);
    Vector3f rightAfterYaw = (yawRotation * Vector4f(m_camera.getRight(), 0.0f)).xyz();
    Matrix4 pitchRotation = Matrix4::RotationAxis(rightAfterYaw, pitch);
    Matrix4 combinedRotation = yawRotation * pitchRotation;

    Vector3f targetToCamera = m_camera.getPosition() - m_target;
    Vector3f rotatedVector = (combinedRotation * Vector4f(targetToCamera, 0.0f)).xyz();

    if (!Math::IsNearlyEqual(rotatedVector.normalized().dot(Vector3f(0.0f, 1.0f, 0.0f)), 1.0f, 0.0001f))
    {
      m_camera.setPosition(m_target + rotatedVector);
      m_camera.lookAt(m_target, Vector3f(0.0f, 1.0f, 0.0f));
    }
  }

  void SceneViewportCameraController::roll()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0 && mouseDelta.y == 0)
      return;

    Angle rollAmount = Angle::FromRadians(-mouseDelta.x * m_cameraRollScale);
    m_camera.roll(rollAmount);
  }

  float SceneViewportCameraController::getCameraDistanceToTarget(const Camera& camera) const
  {
    return (camera.getPosition() - m_target).length();
  }
}
