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
     * @brief Constructs an OpenGL framebuffer.
     */
    OpenGlFrameBuffer();
    ~OpenGlFrameBuffer() override;

    /**
     * @copydoc IFrameBuffer::initialize
     */
    void initialize(UInt32 width, UInt32 height) override;

    /**
     * @copydoc IFrameBuffer::bind
     */
    void bind() override;

    /**
     * @copydoc IFrameBuffer::unbind
     */
    void unbind() override;

    /**
     * @copydoc IFrameBuffer::resize
     */
    void resize(UInt32 width, UInt32 height) override;

    /**
     * @copydoc IFrameBuffer::getColorTexture
     */
    ITexture& getColorTexture() override;

    /**
     * @copydoc IFrameBuffer::getWidth
     */
    UInt32 getWidth() const override;

    /**
     * @copydoc IFrameBuffer::getHeight
     */
    UInt32 getHeight() const override;

    /**
     * @copydoc IFrameBuffer::clear
     */
    void clear(const Color& clearColor) override;

    /**
     * @copydoc IFrameBuffer::isValid
     */
    bool isValid() const override;

    /**
     * @copydoc IFrameBuffer::cleanup
     */
    void cleanup() override;

    /**
     * @copydoc IFrameBuffer::destroy
     */
    void destroy() override;

  private:
    UInt32 m_width;
    UInt32 m_height;
    GLuint m_frameBufferId;
    GLuint m_depthStencilBufferId;
    UniquePtr<OpenGlTexture> m_colorTexture;
    bool m_isValid;
    bool m_isBound;
    GLint m_previousViewport[4];

    void savePreviousViewport();
    void restorePreviousViewport();
  };
}
