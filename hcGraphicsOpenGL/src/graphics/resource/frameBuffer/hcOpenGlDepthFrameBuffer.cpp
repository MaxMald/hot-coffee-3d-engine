#include "hc/graphics/resource/frameBuffer/hcOpenGlDepthFrameBuffer.h"
#include "hc/graphics/resource/texture/hcOpenGlTexture.h"
#include "hc/graphics/resource/textureArray/hcOpenGlTextureArray.h"
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc::graphics::resource::frameBuffer
{
  OpenGlDepthFrameBuffer::OpenGlDepthFrameBuffer() :
    m_openGlFrameBufferId(0),
    m_valid(false)
  {}

  OpenGlDepthFrameBuffer::~OpenGlDepthFrameBuffer()
  {
    destroy();
  }

  void OpenGlDepthFrameBuffer::initialize(
    const ITexture& depthTexture,
    UInt32 mipmapLevel
  )
  {
    destroy();

    if (!depthTexture.isValid())
      throw InvalidArgumentException("Depth texture must be valid to initialize DepthFrameBuffer.");

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    try
    {
      glGenFramebuffers(1, &m_openGlFrameBufferId);
      glBindFramebuffer(GL_FRAMEBUFFER, m_openGlFrameBufferId);

      const OpenGlTexture* openGlDepthTexture
        = reinterpret_cast<const OpenGlTexture*>(&depthTexture);

      glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        openGlDepthTexture->getTextureId(),
        static_cast<GLint>(mipmapLevel)
      );

      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw RuntimeErrorException("Failed to initialize GBuffer: Incomplete framebuffer.");
    }
    catch (...)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
      destroy();
      throw;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);

    m_valid = true;
  }

  void OpenGlDepthFrameBuffer::initialize(
    const textureArray::ITextureArray& depthTexture,
    UInt32 layer,
    UInt32 mipmapLevel
  )
  {
    destroy();

    if (!depthTexture.isValid())
      throw InvalidArgumentException("Depth texture must be valid to initialize DepthFrameBuffer.");

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    try
    {
      glGenFramebuffers(1, &m_openGlFrameBufferId);
      glBindFramebuffer(GL_FRAMEBUFFER, m_openGlFrameBufferId);

      const graphics::resource::textureArray::OpenGlTextureArray* openGlDepthTexture
        = reinterpret_cast<const graphics::resource::textureArray::OpenGlTextureArray*>(&depthTexture);

      glFramebufferTextureLayer(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        openGlDepthTexture->getOpenGLTextureArrayId(),
        static_cast<GLint>(mipmapLevel),
        static_cast<GLint>(layer)
      );

      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw RuntimeErrorException("Failed to initialize GBuffer: Incomplete framebuffer.");
    }
    catch (...)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
      destroy();
      throw;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);

    m_valid = true;
  }

  void OpenGlDepthFrameBuffer::attachDepthTexture(
    const ITexture& depthTexture,
    UInt32 mipmapLevel)
  {
    assertIsValid();

    if (!depthTexture.isValid())
      throw InvalidArgumentException("Depth texture must be valid to attach to DepthFrameBuffer.");

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    try
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_openGlFrameBufferId);

      const OpenGlTexture* openGlDepthTexture
        = reinterpret_cast<const OpenGlTexture*>(&depthTexture);

      glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        openGlDepthTexture->getTextureId(),
        static_cast<GLint>(mipmapLevel)
      );

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw RuntimeErrorException("Failed to attach depth texture: Incomplete framebuffer.");
    }
    catch (...)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
      throw;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
  }

  void OpenGlDepthFrameBuffer::attachDepthTexture(
    const textureArray::ITextureArray& depthTextureArray,
    UInt32 layer,
    UInt32 mipmapLevel
  )
  {
    assertIsValid();

    if (!depthTextureArray.isValid())
      throw InvalidArgumentException("Depth texture array must be valid to attach to DepthFrameBuffer.");

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    try
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_openGlFrameBufferId);

      const graphics::resource::textureArray::OpenGlTextureArray* openGlDepthTextureArray
        = reinterpret_cast<const graphics::resource::textureArray::OpenGlTextureArray*>(&depthTextureArray);

      glFramebufferTextureLayer(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        openGlDepthTextureArray->getOpenGLTextureArrayId(),
        static_cast<GLint>(mipmapLevel),
        static_cast<GLint>(layer)
      );

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw RuntimeErrorException("Failed to attach depth texture array: Incomplete framebuffer.");
    }
    catch (...)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
      throw;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
  }

  void OpenGlDepthFrameBuffer::bind(frameBufferBindingType::Type bindingType)
  {
    assertIsValid();
    switch (bindingType)
    {
    case frameBufferBindingType::Read:
      glBindFramebuffer(GL_READ_FRAMEBUFFER, m_openGlFrameBufferId);
      break;
    case frameBufferBindingType::Draw:
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_openGlFrameBufferId);
      break;
    case frameBufferBindingType::ReadDraw:
      glBindFramebuffer(GL_FRAMEBUFFER, m_openGlFrameBufferId);
      break;
    default:
      throw InvalidArgumentException("Invalid binding type specified for DepthFrameBuffer.");
    }
  }

  void OpenGlDepthFrameBuffer::unbind(frameBufferBindingType::Type bindingType)
  {
    assertIsValid();
    switch (bindingType)
    {
    case frameBufferBindingType::Read:
      glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      break;
    case frameBufferBindingType::Draw:
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
      break;
    case frameBufferBindingType::ReadDraw:
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      break;
    default:
      throw InvalidArgumentException("Invalid binding type specified for DepthFrameBuffer.");
    }
  }

  bool OpenGlDepthFrameBuffer::isBound(frameBufferBindingType::Type bindingType) const
  {
    assertIsValid();
    GLint currentFramebuffer = 0;
    switch (bindingType)
    {
    case frameBufferBindingType::Read:
      glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentFramebuffer);
      return currentFramebuffer == static_cast<GLint>(m_openGlFrameBufferId);
    case frameBufferBindingType::Draw:
      glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentFramebuffer);
      return currentFramebuffer == static_cast<GLint>(m_openGlFrameBufferId);
    case frameBufferBindingType::ReadDraw:
      glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFramebuffer);
      return currentFramebuffer == static_cast<GLint>(m_openGlFrameBufferId);
    default:
      throw InvalidArgumentException("Invalid binding type specified for DepthFrameBuffer.");
    }
  }

  void OpenGlDepthFrameBuffer::copyDepthTo(
    IFrameBuffer& destinationFrameBuffer,
    const FrameBufferCopySettings& copySettings
  )
  {
    assertIsValid();
    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    try
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, m_openGlFrameBufferId);
      destinationFrameBuffer.bindForDrawingOnly();
      glBlitFramebuffer(
        copySettings.sourceX, copySettings.sourceY,
        copySettings.sourceWidth, copySettings.sourceHeight,
        copySettings.destX, copySettings.destY,
        copySettings.destWidth, copySettings.destHeight,
        GL_DEPTH_BUFFER_BIT,
        openGlGraphicsUtilities::GetOpenGlFilterMode(copySettings.filter)
      );
    }
    catch (...)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
      throw;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
  }

  void OpenGlDepthFrameBuffer::clear()
  {
    assertIsValid();
    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, m_openGlFrameBufferId);
    glClear(GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
  }

  void OpenGlDepthFrameBuffer::destroy()
  {
    if (m_openGlFrameBufferId != 0)
    {
      GLint currentReadFrameBuffer = 0;
      GLint currentDrawFrameBuffer = 0;

      glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
      glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

      if (currentReadFrameBuffer == static_cast<GLint>(m_openGlFrameBufferId))
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      if (currentDrawFrameBuffer == static_cast<GLint>(m_openGlFrameBufferId))
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

      glDeleteFramebuffers(1, &m_openGlFrameBufferId);
      m_openGlFrameBufferId = 0;
    }
    m_valid = false;
  }
}
