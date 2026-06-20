#include "hc/editor/views/windows/sceneViewport/hcSceneViewportGizmoController.h"

namespace hc::editor
{
  static constexpr float CUBE_VIEW_SIZE = 128.0f;

  SceneViewportGizmoController::SceneViewportGizmoController(
    InputManager& inputManager,
    SceneViewportCamera& camera
  ) :
    m_inputManager(inputManager),
    m_activeGameObject(nullptr),
    m_camera(camera),
    m_currentOperation(ImGuizmo::OPERATION::TRANSLATE),
    m_currentMode(ImGuizmo::MODE::LOCAL)
  {
  }

  SceneViewportGizmoController::~SceneViewportGizmoController()
  {}

  void SceneViewportGizmoController::update(const Time& elapsedTime)
  {
    if (m_inputManager.getKeyboardKeyState(keyboardKey::T).isPressed())
      m_currentOperation = ImGuizmo::OPERATION::TRANSLATE;
    else if (m_inputManager.getKeyboardKeyState(keyboardKey::R).isPressed())
      m_currentOperation = ImGuizmo::OPERATION::ROTATE;
    else if (m_inputManager.getKeyboardKeyState(keyboardKey::S).isPressed())
      m_currentOperation = ImGuizmo::OPERATION::SCALE;
    else if (m_inputManager.getKeyboardKeyState(keyboardKey::Q).isPressed())
      m_currentMode = (m_currentMode == ImGuizmo::MODE::LOCAL) ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL;
  }

  void SceneViewportGizmoController::draw(
    const Vector2f& windowPosition,
    const Vector2f& windowSize
  )
  {
    if (windowSize.x <= 0 || windowSize.y <= 0)
      return;

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(
      windowPosition.x,
      windowPosition.y,
      windowSize.x,
      windowSize.y
    );

    drawCubeView(windowPosition, windowSize);

    if (m_activeGameObject)
      drawGizmo(windowPosition, windowSize);
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

  void SceneViewportGizmoController::drawGizmo(
    const Vector2f& windowPosition,
    const Vector2f& windowSize
  )
  {
    Matrix4 view = m_camera.getCamera().getViewMatrix();
    Matrix4 projection = m_camera.getCamera().getProjectionMatrix();

    // ImGuizmo expects column-major matrices, so we need to transpose them
    view.transpose();
    projection.transpose();

    Vector3f position = m_activeGameObject->getPosition();
    Vector3f rotation = m_activeGameObject->getRotation() * Math::RadToDeg;
    Vector3f scale = m_activeGameObject->getScale();
    Matrix4 composedMatrix;

    ImGuizmo::RecomposeMatrixFromComponents(
      &(position.x),
      &(rotation.x),
      &(scale.x),
      composedMatrix.m[0]
    );

    bool changed = ImGuizmo::Manipulate(
      view.m[0],
      projection.m[0],
      m_currentOperation,
      m_currentMode,
      composedMatrix.m[0]
    );

    if (!changed)
      return;

     ImGuizmo::DecomposeMatrixToComponents(
      composedMatrix.m[0],
      &(position.x),
      &(rotation.x),
      &(scale.x)
    );

    m_activeGameObject->setPosition(position);
    Vector3f deltaRotation = (rotation * Math::DegToRad) - m_activeGameObject->getRotation();
    m_activeGameObject->setRotation(m_activeGameObject->getRotation() + deltaRotation);
    m_activeGameObject->setScale(scale);
  }

  void SceneViewportGizmoController::drawCubeView(
    const Vector2f& windowPosition,
    const Vector2f& windowSize
  )
  {
    Matrix4 view = m_camera.getCamera().getViewMatrix();
    view.transpose();

    ImGuizmo::ViewManipulate(
      view.m[0],
      8.0f,
      ImVec2(windowPosition.x + windowSize.x - CUBE_VIEW_SIZE, windowPosition.y),
      ImVec2(CUBE_VIEW_SIZE, CUBE_VIEW_SIZE),
      0x10101000
    );
    
    Vector3f direction;
    direction.x = -view.m02;
    direction.y = -view.m12;
    direction.z = -view.m22;

    Vector3f target = m_camera.getTarget();
    Vector3f cameraPosition = target - direction * m_camera.getDistanceToTarget();
    m_camera.setCameraPosition(cameraPosition);
  }
}
