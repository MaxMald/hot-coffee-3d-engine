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
  class SceneViewportCameraController
  {
  public:
    /**
     * @brief Constructs a camera controller.
     * 
     * @param inputManager The input manager for handling user input.
     */
    SceneViewportCameraController(InputManager& inputManager);
    virtual ~SceneViewportCameraController() = default;

    /**
     * @brief Gets the camera controlled by this controller.
     * 
     * @return Reference to the controlled camera.
     */
    SceneViewportCamera& getCamera();

    /**
     * @brief Prepares the camera controller for use.
     */
    void prepare();

    /**
     * @brief Updates the camera based on user input.
     * 
     * @param elapsedTime The time elapsed since the last update.
     */
    void update(const Time& elapsedTime);

  private:
    SceneViewportCamera m_camera;
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
