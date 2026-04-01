#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include <GL/glew.h>

namespace hc
{
  class OpenGlTexture;

  /**
   * @brief OpenGL implementation of the framebuffer interface.
   *
   * Manages an OpenGL framebuffer object with color texture and
   * depth-stencil attachments for off-screen rendering.
   */
  class OpenGlFrameBuffer : public IFrameBuffer
  {
  public:
    /**
     * @brief Constructs an OpenGL framebuffer with the specified
     * dimensions.
     *
     * @param width The width in pixels.
     * @param height The height in pixels.
     */
    OpenGlFrameBuffer(
      UInt32 width,
      UInt32 height
    );
    ~OpenGlFrameBuffer() override;

    /**
     * @brief Binds this framebuffer as the current render target.
     */
    void bind() override;

    /**
     * @brief Unbinds this framebuffer, restoring the default render
     * target.
     */
    void unbind() override;

    /**
     * @brief Resizes the framebuffer to the specified dimensions.
     *
     * @param width The new width in pixels.
     * @param height The new height in pixels.
     */
    void resize(UInt32 width, UInt32 height) override;

    /**
     * @brief Gets the color texture attached to this framebuffer.
     *
     * @return Reference to the color texture.
     */
    ITexture& getColorTexture() override;

    /**
     * @brief Gets the width of the framebuffer.
     *
     * @return The width in pixels.
     */
    UInt32 getWidth() const override;

    /**
     * @brief Gets the height of the framebuffer.
     *
     * @return The height in pixels.
     */
    UInt32 getHeight() const override;

    /**
     * @brief Checks if the framebuffer is valid and ready for use.
     *
     * @return True if the framebuffer is valid, false otherwise.
     */
    bool isValid() const override;

    /**
     * @brief Destroys the framebuffer and releases all OpenGL resources.
     */
    void destroy() override;

  private:
    UInt32 m_width;
    UInt32 m_height;
    GLuint m_frameBufferId;
    GLuint m_depthStencilBufferId;
    UniquePtr<OpenGlTexture> m_colorTexture;
    bool m_isValid;
  };
}
