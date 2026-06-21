#include "hc/editor/views/windows/sceneViewport/hcSceneViewportCameraInputController.h"

namespace hc::editor
{
  SceneViewportCameraInputController::SceneViewportCameraInputController(
    SceneViewportCamera& camera,
    InputManager& inputManager
  ) :
    m_camera(camera),
    m_inputManager(inputManager)
  {}

  void SceneViewportCameraInputController::update(const Time&)
  {
    if (isMouseMiddleButtonPressed())
    {
      if (isShiftKeyPressed())
        localXYMovement();
      else if (isAltKeyPressed())
        roll();
      else
        orbit();
    }
    else if (isScrollingVertically())
    {
      if (isShiftKeyPressed())
        localZMovement();
      else
        zoom();
    }
  }

  bool SceneViewportCameraInputController::isMouseMiddleButtonPressed() const
  {
    return m_inputManager.isMouseButtonPressed(mouseButtonKey::Middle);
  }

  bool SceneViewportCameraInputController::isShiftKeyPressed() const
  {
    return m_inputManager.isKeyboardKeyPressed(keyboardKey::LShift) ||
      m_inputManager.isKeyboardKeyPressed(keyboardKey::RShift);
  }

  bool SceneViewportCameraInputController::isAltKeyPressed() const
  {
    return m_inputManager.isKeyboardKeyPressed(keyboardKey::LAlt) ||
      m_inputManager.isKeyboardKeyPressed(keyboardKey::RAlt);
  }

  bool SceneViewportCameraInputController::isScrollingVertically() const
  {
    float scrollDelta = m_inputManager
      .getMouseState()
      .getScrollState()
      .getVerticalScrollDelta();

    return scrollDelta != 0.0f;
  }

  void SceneViewportCameraInputController::localXYMovement()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    m_camera.truck(-mouseDelta.x);
    m_camera.pedestal(mouseDelta.y);
  }

  void SceneViewportCameraInputController::localZMovement()
  {
    float scrollDelta = m_inputManager
      .getMouseState()
      .getScrollState()
      .getVerticalScrollDelta();
    m_camera.dolly(scrollDelta);
  }

  void SceneViewportCameraInputController::zoom()
  {
    float scrollDelta = m_inputManager
      .getMouseState()
      .getScrollState()
      .getVerticalScrollDelta();
    m_camera.zoom(scrollDelta);
  }

  void SceneViewportCameraInputController::orbit()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0 && mouseDelta.y == 0)
      return;

    m_camera.orbit(
      Vector2f(
        static_cast<float>(mouseDelta.x),
        static_cast<float>(mouseDelta.y)
      )
    );
  }

  void SceneViewportCameraInputController::roll()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0)
      return;

    Angle rollAmount = Angle::FromRadians(-mouseDelta.x);
    m_camera.roll(rollAmount);
  }
}
