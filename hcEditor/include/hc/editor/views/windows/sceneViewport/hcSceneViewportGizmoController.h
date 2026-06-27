#pragma once

#include <ImGuizmo.h>
#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/views/windows/sceneViewport/hcSceneViewportCamera.h"

namespace hc::editor
{
  class SceneViewportGizmoController
  {
  public:
    SceneViewportGizmoController(InputManager& inputManager, SceneViewportCamera& camera);
    ~SceneViewportGizmoController();

    void update(const Time& elapsedTime);
    void draw(const Vector2f& windowPosition, const Vector2f& windowSize);
    void setActiveGameObject(GameObject* gameObject);
    bool hasActiveGameObject() const;
    GameObject* getActiveGameObject() const;
    void clearActiveGameObject();
    UInt32 getGizmoMode() const;
    void setGizmoMode(UInt32 mode);
    UInt32 getGizmoOperation() const;
    void setGizmoOperation(UInt32 operation);
    bool isDrawingGrid() const;
    void setDrawingGrid(bool drawingGrid);
    float getGridSize() const;
    void setGridSize(float gridSize);
    bool isUsingSnap() const;
    void setUsingSnap(bool usingSnap);
    Vector3f getSnapValues() const;
    void setSnapValues(const Vector3f& snapValues);

  private:
    InputManager& m_inputManager;
    GameObject* m_activeGameObject;
    SceneViewportCamera& m_camera;
    ImGuizmo::OPERATION m_currentOperation;
    ImGuizmo::MODE m_currentMode;
    Vector3f m_snapValues;
    bool m_usingSnap;
    bool m_drawingGrid;
    float m_gridSize;

    void drawGizmo(const Vector2f& windowPosition, const Vector2f& windowSize);
    void drawCubeView(const Vector2f& windowPosition, const Vector2f& windowSize);
    void drawGrid();
  };
}
