#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

namespace hc
{
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
     * @copydoc IFrameBuffer::bindForReadingOnly
     */
    void bindForReadingOnly() override;

    /**
     * @copydoc IFrameBuffer::bindForDrawingOnly
     */
    void bindForDrawingOnly() override;

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
     * @copydoc IFrameBuffer::copyDepthTo
     */
    void copyDepthTo(IFrameBuffer& destinationFrameBuffer) override;

    /**
     * @copydoc IFrameBuffer::destroySelf
     */
    void destroySelf() override;

  private:
    UInt32 m_width;
    UInt32 m_height;
    UInt32 m_frameBufferId;
    UInt32 m_depthStencilBufferId;
    OpenGlTexture m_colorTexture;
    bool m_valid;

    void assertValid() const;
  };
}
