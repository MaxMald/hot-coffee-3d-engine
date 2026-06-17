#include "hc/editor/views/windows/sceneViewport/hcSceneViewportCameraController.h"
#include "hc/editor/scenes/hcEditorSceneNames.h"

namespace hc::editor
{
  SceneViewportCameraController::SceneViewportCameraController(
    InputManager& inputManager
  ) :
    m_camera(),
    m_inputManager(inputManager)
  {}

  SceneViewportCamera& SceneViewportCameraController::getCamera()
  {
    return m_camera;
  }

  void SceneViewportCameraController::prepare()
  {
    m_camera.setCameraPosition(Vector3f(0.0f, 0.0f, 5.0f));
  }

  void SceneViewportCameraController::update(const Time&)
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

  void SceneViewportCameraController::localXYMovement()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    m_camera.truck(-mouseDelta.x);
    m_camera.pedestal(mouseDelta.y);
  }

  void SceneViewportCameraController::localZMovement()
  {
    float scrollDelta = m_inputManager
      .getMouseState()
      .getScrollState()
      .getVerticalScrollDelta();
    m_camera.dolly(scrollDelta);
  }

  void SceneViewportCameraController::zoom()
  {
    float scrollDelta = m_inputManager
      .getMouseState()
      .getScrollState()
      .getVerticalScrollDelta();
    m_camera.zoom(scrollDelta);
  }

  void SceneViewportCameraController::orbit()
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

  void SceneViewportCameraController::roll()
  {
    Vector2i mouseDelta = m_inputManager.getMouseState().getDeltaPosition();
    if (mouseDelta.x == 0)
      return;

    Angle rollAmount = Angle::FromRadians(-mouseDelta.x);
    m_camera.roll(rollAmount);
  }
}
