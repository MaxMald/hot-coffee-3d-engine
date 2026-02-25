#pragma once

#include "hc/editor/services/hcIEditorService.h"
#include "hc/editor/services/hcIUpdatableEditorService.h"

namespace hc::editor
{
  class EditorCameraController :
    public IEditorService,
    public IUpdatableEditorService
  {
  public:
    EditorCameraController(
      SceneManager& sceneManager,
      InputManager& inputManager
    );
    virtual ~EditorCameraController() = default;

    void update(const Time& elapsedTime) override;
    void destroy() override;

  private:
    SceneManager& m_sceneManager;
    InputManager& m_inputManager;
    float m_cameraMoveScale;
    float m_cameraZoomScale;
    float m_cameraRollScale;
    float m_minDistanceToTarget;
    Vector3f m_target;

    bool isMouseMiddleButtonPressed() const;
    bool isShiftKeyPressed() const;
    bool isAltKeyPressed() const;
    bool isScrollingVertically() const;

    void localMove();
    void zoom();
    void orbit();
    void roll();

    Camera& getActiveCamera();
    float getCameraDistanceToTarget(const Camera& camera) const;
  };;
}
