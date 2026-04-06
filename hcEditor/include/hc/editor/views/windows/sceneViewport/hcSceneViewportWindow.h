#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"
#include "hc/editor/views/windows/sceneViewport/hcSceneViewportCameraController.h"
#include "hc/editor/views/windows/sceneViewport/hcSceneViewportRenderer.h"
#include "hc/editor/services/gameObjectSelection/hcGameObjectSelectionService.h"
#include "hc/editor/services/gameObjectSelection/hcIGameObjectSelectionServiceListener.h"

namespace hc::editor
{
  /**
   * @brief Window view that displays a 3D scene with camera controls.
   *
   * Provides an interactive viewport for viewing and navigating the
   * scene using camera controls.
   */
  class SceneViewportWindow :
    public AWindowView,
    public IGameObjectSelectionServiceListener
  {
  public:
    /**
     * @brief Constructs a scene viewport window.
     * 
     * @param engine The engine instance for accessing scene and graphics.
     * @param selectionService The game object selection service to listen for selection
     * changes.
     */
    SceneViewportWindow(
      HotCoffeeEngine& engine,
      GameObjectSelectionService& selectionService
    );

    /**
     * @brief Destroys the viewport window and releases resources.
     */
    void destroy() override;

  protected:
    HotCoffeeEngine& m_engine;
    GameObjectSelectionService& m_selectionService;
    SceneViewportRenderer m_renderer;
    SceneViewportCameraController m_cameraController;
    Vector2f m_uvTopLeft;
    Vector2f m_uvBottomRight;

    /**
     * @brief Updates the camera controller and viewport state.
     * 
     * @param elapsedTime The time elapsed since the last update.
     */
    void onUpdate(const Time& elapsedTime) override;

    /**
     * @brief Draws the viewport window with the rendered scene.
     */
    void onDraw() override;

    /**
     * @copydoc IGameObjectSelectionServiceListener::onGameObjectSelected
     */
    void onGameObjectSelected(GameObject* gameObject) override;

    /**
     * @copydoc IGameObjectSelectionServiceListener::onGameObjectDeselected
     */
    void onGameObjectDeselected(GameObject* gameObject) override;

    /**
     * @brief Updates framebuffer size to match content region.
     */
    void updateFramebufferSize();

    /**
     * @brief Renders the content scene to the render texture.
     */
    void renderSceneToTexture();

    /**
     * @brief Draws the viewport window with the rendered scene texture.
     */
    void drawViewport();
  };
}
