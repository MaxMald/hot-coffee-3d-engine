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
    m_currentMode(ImGuizmo::MODE::LOCAL),
    m_snapValues(1.0f, 1.0f, 1.0f),
    m_usingSnap(false),
    m_drawingGrid(false),
    m_gridSize(100.0f)
  {}

  SceneViewportGizmoController::~SceneViewportGizmoController()
  {}

  void SceneViewportGizmoController::update(const Time& elapsedTime)
  {
    if (m_inputManager.getKeyboardKeyState(keyboardKey::T).isPressed())
      m_currentOperation = ImGuizmo::OPERATION::TRANSLATE;
    else if (m_inputManager.getKeyboardKeyState(keyboardKey::E).isPressed())
      m_currentOperation = ImGuizmo::OPERATION::ROTATE;
    else if (m_inputManager.getKeyboardKeyState(keyboardKey::R).isPressed())
      m_currentOperation = ImGuizmo::OPERATION::SCALE;
  }

  void SceneViewportGizmoController::draw(
    const Vector2f& windowPosition,
    const Vector2f& windowSize
  )
  {
    if (windowSize.x <= 0 || windowSize.y <= 0)
      return;

    if (m_camera.getCameraProjectionType() == projectionType::Type::Orthographic)
      ImGuizmo::SetOrthographic(true);
    else
      ImGuizmo::SetOrthographic(false);

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(
      windowPosition.x,
      windowPosition.y,
      windowSize.x,
      windowSize.y
    );

    drawCubeView(windowPosition, windowSize);

    if (m_drawingGrid)
      drawGrid();

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

  UInt32 SceneViewportGizmoController::getGizmoMode() const
  {
    return static_cast<UInt32>(m_currentMode);
  }

  void SceneViewportGizmoController::setGizmoMode(UInt32 mode)
  {
    m_currentMode = static_cast<ImGuizmo::MODE>(mode);
  }

  UInt32 SceneViewportGizmoController::getGizmoOperation() const
  {
    return static_cast<UInt32>(m_currentOperation);
  }

  void SceneViewportGizmoController::setGizmoOperation(UInt32 operation)
  {
    m_currentOperation = static_cast<ImGuizmo::OPERATION>(operation);
  }

  bool SceneViewportGizmoController::isDrawingGrid() const
  {
    return m_drawingGrid;
  }

  void SceneViewportGizmoController::setDrawingGrid(bool drawingGrid)
  {
    m_drawingGrid = drawingGrid;
  }

  float SceneViewportGizmoController::getGridSize() const
  {
    return m_gridSize;
  }

  void SceneViewportGizmoController::setGridSize(float gridSize)
  {
    m_gridSize = gridSize;
  }

  bool SceneViewportGizmoController::isUsingSnap() const
  {
    return m_usingSnap;
  }

  void SceneViewportGizmoController::setUsingSnap(bool usingSnap)
  {
    m_usingSnap = usingSnap;
  }

  Vector3f SceneViewportGizmoController::getSnapValues() const
  {
    return m_snapValues;
  }

  void SceneViewportGizmoController::setSnapValues(const Vector3f& snapValues)
  {
    m_snapValues = snapValues;
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
      composedMatrix.m[0],
      nullptr,
      m_usingSnap ? &(m_snapValues.x) : nullptr
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
    m_activeGameObject->setRotation(rotation * Math::DegToRad);
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

  void SceneViewportGizmoController::drawGrid()
  {
    Matrix4 view = m_camera.getCamera().getViewMatrix();
    view.transpose();
    Matrix4 projection = m_camera.getCamera().getProjectionMatrix();
    projection.transpose();
    Matrix4 identityMatrix = Matrix4::Identity();

    ImGuizmo::DrawGrid(
      view.m[0],
      projection.m[0],
      identityMatrix.m[0],
      m_gridSize
    );
  }
}
