#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief Renders a scene to an off-screen framebuffer for viewport
   * display.
   *
   * Manages a framebuffer for rendering the 3D scene from a specified
   * camera, producing a texture that can be displayed in the editor UI.
   */
  class SceneViewportRenderer
  {
  public:
    /**
     * @brief Constructs a scene viewport renderer.
     * 
     * @param graphicsManager The graphics manager for rendering operations.
     */
    SceneViewportRenderer(IGraphicsManager& graphicsManager);
    ~SceneViewportRenderer();

    /**
     * @brief Resizes the viewport framebuffer to the specified dimensions.
     * 
     * @param width The new width in pixels.
     * @param height The new height in pixels.
     */
    void resize(UInt32 width, UInt32 height);

    /**
     * @brief Gets the current width of the viewport framebuffer.
     *
     * @return The width in pixels.
     */
    UInt32 getWidth() const;

    /**
     * @brief Gets the current height of the viewport framebuffer.
     *
     * @return The height in pixels.
     */
    UInt32 getHeight() const;

    /**
     * @brief Renders the scene from the specified camera.
     * 
     * @param scene The scene to render.
     * @param camera The camera to render from.
     */
    void renderScene(Scene& scene, Camera& camera);

    /**
     * @brief Gets the texture containing the rendered scene.
     * 
     * @return Reference to the rendered texture.
     */
    ITexture& getRenderedTexture() const;

    /**
     * @brief Gets the background clear color for the viewport.
     * 
     * @return The current clear color.
     */
    const Color& getClearColor() const;

    /**
     * @brief Sets the background clear color for the viewport.
     * 
     * @param color The new clear color.
     */
    void setClearColor(const Color& color);

    /**
     * @brief Checks if the renderer is valid and ready for use.
     * 
     * @return True if the framebuffer is valid, false otherwise.
     */
    bool isValid() const;

  private:
    IGraphicsManager& m_graphicsManager;
    FrameBufferPtr m_frameBuffer;
    Color m_clearColor;

    void assertFrameBufferValid() const;
  };
}
