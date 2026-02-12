#pragma once

#include "hc/editor/hcIEditorService.h"
#include "hc/editor/hcIUpdatableEditorService.h"

namespace hc::editor
{
  class EditorCameraController :
    public IEditorService,
    public IUpdatableEditorService
  {
  public:
    EditorCameraController(SceneManager& sceneManager);
    virtual ~EditorCameraController() = default;

    void update(const Time& elapsedTime) override;
    void destroy() override;

  private:
    SceneManager& m_sceneManager;

    float m_cameraMoveSpeed;
    float m_cameraRotationSpeed;
  };;
}
