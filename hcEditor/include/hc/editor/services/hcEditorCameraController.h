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
    float m_cameraDollyScale;
    float m_cameraRotationSpeed;

    bool receivedMoveCommand();
    void move();
    bool receivedDollyCommand();
    void dolly();
    Camera& getActiveCamera();
  };;
}
