#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/views/windows/sceneViewport/hcSceneViewportCamera.h"

namespace hc::editor
{
  /**
   * @brief Controls camera movement in the scene viewport.
   *
   * Provides orbit, zoom, pan, and roll controls for navigating
   * the 3D scene using mouse and keyboard input.
   */
  class SceneViewportCameraInputController
  {
  public:
    /**
     * @brief Constructs a camera controller.
     *
     * @param camera The scene viewport camera to control.
     * @param inputManager The input manager for handling user input.
     */
    SceneViewportCameraInputController(SceneViewportCamera& camera, InputManager& inputManager);
    virtual ~SceneViewportCameraInputController() = default;

    /**
     * @brief Updates the camera based on user input.
     * 
     * @param elapsedTime The time elapsed since the last update.
     */
    void update(const Time& elapsedTime);

  private:
    SceneViewportCamera& m_camera;
    InputManager& m_inputManager;

    bool isMouseMiddleButtonPressed() const;
    bool isShiftKeyPressed() const;
    bool isAltKeyPressed() const;
    bool isScrollingVertically() const;
    void localXYMovement();
    void localZMovement();
    void zoom();
    void orbit();
    void roll();
  };
}
