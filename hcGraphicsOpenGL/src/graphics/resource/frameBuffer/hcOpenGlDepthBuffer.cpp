#include "hc/graphics/resource/frameBuffer/hcOpenGlDepthBuffer.h"

namespace hc
{
  OpenGlDepthBuffer::OpenGlDepthBuffer() :
    m_depthTexture(),
    m_depthBufferId(0),
    m_width(0),
    m_height(0),
    m_valid(false)
  {}

  OpenGlDepthBuffer::~OpenGlDepthBuffer()
  {
    destroy();
  }

  void OpenGlDepthBuffer::initialize(UInt32 width, UInt32 height)
  {
    if (m_valid)
      throw RuntimeErrorException("DepthBuffer is already initialized.");

    if (width == 0 || height == 0)
      throw InvalidArgumentException("DepthBuffer dimensions must be greater than zero.");

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    try
    {
      glGenFramebuffers(1, &m_depthBufferId);
      glBindFramebuffer(GL_FRAMEBUFFER, m_depthBufferId);

      m_depthTexture.initialize(
        width, height,
        textureFormatType::Depth24,
        colorSpaceType::Linear,
        nullptr
      );

      if (!m_depthTexture.isValid())
        throw RuntimeErrorException("Failed to create depth texture for DepthBuffer.");

      glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        m_depthTexture.getTextureId(),
        0
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

    m_width = width;
    m_height = height;
    m_valid = true;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
  }

  void OpenGlDepthBuffer::bind()
  {
    assertIsValid();
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthBufferId);
  }

  void OpenGlDepthBuffer::bindForReadingOnly()
  {
    assertIsValid();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_depthBufferId);
  }

  void OpenGlDepthBuffer::bindForDrawingOnly()
  {
    assertIsValid();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_depthBufferId);
  }
  void OpenGlDepthBuffer::unbind()
  {
    assertIsValid();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGlDepthBuffer::resize(UInt32 width, UInt32 height)
  {
    assertIsValid();

    if (width == 0 || height == 0)
      throw InvalidArgumentException("DepthBuffer dimensions must be greater than zero.");

    if (width == m_width && height == m_height)
      return;

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    try
    {
      m_depthTexture.resize(width, height);

      glBindFramebuffer(GL_FRAMEBUFFER, m_depthBufferId);
      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw RuntimeErrorException("Failed to resize DepthBuffer: Incomplete framebuffer.");
    }
    catch (...)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
      destroy();
      throw;
    }

    m_width = width;
    m_height = height;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
  }

  ITexture& OpenGlDepthBuffer::getColorTexture()
  {
    throw RuntimeErrorException("DepthBuffer does not have a color texture.");
  }

  UInt32 OpenGlDepthBuffer::getWidth() const
  {
    return m_width;
  }

  UInt32 OpenGlDepthBuffer::getHeight() const
  {
    return m_height;
  }

  void OpenGlDepthBuffer::clear(const Color&)
  {
    assertIsValid();

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    try
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_depthBufferId);
      glClear(GL_DEPTH_BUFFER_BIT);
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
  }

  bool OpenGlDepthBuffer::isValid() const
  {
    return m_valid;
  }

  void OpenGlDepthBuffer::destroy()
  {
    if (m_depthBufferId != 0)
    {
      GLint currentReadFramebuffer = 0;
      glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFramebuffer);
      if (currentReadFramebuffer == static_cast<GLint>(m_depthBufferId))
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

      GLint currentDrawFramebuffer = 0;
      glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFramebuffer);
      if (currentDrawFramebuffer == static_cast<GLint>(m_depthBufferId))
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

      glDeleteFramebuffers(1, &m_depthBufferId);
      m_depthBufferId = 0;
    }

    m_depthTexture.destroy();
    m_width = 0;
    m_height = 0;
    m_valid = false;
  }

  void OpenGlDepthBuffer::copyDepthTo(IFrameBuffer & destinationFrameBuffer)
  {
    assertIsValid();

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    try
    {
      bindForReadingOnly();
      destinationFrameBuffer.bindForDrawingOnly();
      glBlitFramebuffer(
        0, 0, m_width, m_height,
        0, 0, destinationFrameBuffer.getWidth(), destinationFrameBuffer.getHeight(),
        GL_DEPTH_BUFFER_BIT,
        GL_NEAREST
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

  void OpenGlDepthBuffer::destroySelf()
  {
    delete this;
  }

  ITexture& OpenGlDepthBuffer::getDepthTexture()
  {
    return m_depthTexture;
  }

  void OpenGlDepthBuffer::assertIsValid() const
  {
    if (!m_valid)
      throw RuntimeErrorException("DepthBuffer is not valid.");
  }
}
