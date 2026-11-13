#pragma once

#include <GL/glew.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc::graphics::resource::frameBuffer
{
  /**
   * @brief OpenGL implementation of a depth frame buffer.
   */
  class OpenGlDepthFrameBuffer : public IDepthFrameBuffer
  {
  public:
    OpenGlDepthFrameBuffer();
    ~OpenGlDepthFrameBuffer() override;

    /**
     * @copydoc IDepthFrameBuffer::initialize(const ITexture&, UInt32)
     */
    void initialize(const ITexture& depthTexture, UInt32 mipmapLevel = 0) override;

    /**
     * @copydoc IDepthFrameBuffer::initialize(const textureArray::ITextureArray&, UInt32, UInt32)
     */
    void initialize(
      const textureArray::ITextureArray& depthTextureArray,
      UInt32 layer,
      UInt32 mipmapLevel = 0
    ) override;

    /**
     * @copydoc IDepthFrameBuffer::attachDepthTexture(const ITexture&, UInt32)
     */
    void attachDepthTexture(const ITexture& depthTexture, UInt32 mipmapLevel = 0) override;

    /**
     * @copydoc IDepthFrameBuffer::attachDepthTexture(const textureArray::ITextureArray&, UInt32, UInt32)
     */
    void attachDepthTexture(
      const textureArray::ITextureArray& depthTextureArray,
      UInt32 layer,
      UInt32 mipmapLevel = 0
    ) override;

    /**
     * @copydoc IDepthFrameBuffer::bind(frameBufferBindingType::Type)
     */
    void bind(
      frameBufferBindingType::Type bindingType = frameBufferBindingType::ReadDraw
    );

    /**
     * @copydoc IDepthFrameBuffer::unbind(frameBufferBindingType::Type)
     */
    void unbind(
      frameBufferBindingType::Type bindingType = frameBufferBindingType::ReadDraw
    ) override;

    /**
     * @copydoc IDepthFrameBuffer::isBound(frameBufferBindingType::Type) const
     */
    bool isBound(frameBufferBindingType::Type bindingType) const override;

    /**
     * @copydoc IDepthFrameBuffer::copyDepthTo(IFrameBuffer&, const FrameBufferCopySettings&) const
     */
    void copyDepthTo(
      IFrameBuffer& destinationFrameBuffer,
      const FrameBufferCopySettings& copySettings
    ) override;

    /**
     * @copydoc IDepthFrameBuffer::clear()
     */
    void clear() override;

    /**
     * @copydoc IGraphicResource::isValid() const
     */
    bool isValid() const override
    {
      return m_valid;
    }

    /**
     * @copydoc IGraphicResource::destroy()
     */
    void destroy() override;

  private:
    GLuint m_openGlFrameBufferId;
    bool m_valid;

    /**
     * @brief Asserts that the depth buffer is valid. Throws a RuntimeErrorException if it
     * is not valid.
     */
    inline void assertIsValid() const
    {
      if (!m_valid)
        throw RuntimeErrorException("DepthFrameBuffer is not valid.");
    }
  };
}
