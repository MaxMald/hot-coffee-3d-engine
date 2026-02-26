#pragma once

#include "hc/editor/services/hcIEditorService.h"
#include "hc/editor/services/hcIUpdatableEditorService.h"

namespace hc::editor
{
  /**
   * @brief Service responsible for controlling the editor camera based on user
   * input. Provides camera movement, zooming, orbiting, and rolling
   * capabilities.
   */
  class EditorCameraController :
    public IEditorService,
    public IUpdatableEditorService
  {
  public:
    /**
     * @brief Constructs the editor camera controller.
     * 
     * @param sceneManager Reference to the scene manager.
     * @param inputManager Reference to the input manager.
     */
    EditorCameraController(
      SceneManager& sceneManager,
      InputManager& inputManager
    );
    virtual ~EditorCameraController() = default;

    /**
     * @copydoc IEditorService::prepare
     */
    void prepare() override;

    /**
     * @copydoc IUpdatableEditorService::update
     */
    void update(const Time& elapsedTime) override;

    /**
     * @copydoc IEditorService::destroy
     */
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
  };
}
