#include "hc/graphics/hcOpenGlFrameBuffer.h"
#include <GL/glew.h>

namespace hc
{
  OpenGlFrameBuffer::OpenGlFrameBuffer() :
    m_width(0),
    m_height(0),
    m_frameBufferId(0),
    m_depthStencilBufferId(0),
    m_colorTexture(),
    m_valid(false)
  {
  }

  OpenGlFrameBuffer::~OpenGlFrameBuffer()
  {
    destroy();
  }

  void OpenGlFrameBuffer::initialize(UInt32 width, UInt32 height)
  {
    if (m_valid)
      throw RuntimeErrorException("Framebuffer is already initialized");

    if (width == 0 || height == 0)
      throw InvalidArgumentException(
        "Framebuffer dimensions must be greater than zero"
      );

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    GLint currentRenderbuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRenderbuffer);

    try
    {
      m_colorTexture.initialize(
        width, height,
        GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE
      );

      if (!m_colorTexture.isValid())
        throw RuntimeErrorException("Failed to create color texture for framebuffer");

      glGenFramebuffers(1, &m_frameBufferId);
      glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
      glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        m_colorTexture.getTextureId(),
        0
      );

      glGenRenderbuffers(1, &m_depthStencilBufferId);
      glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBufferId);
      glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
        width, height
      );

      glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
        m_depthStencilBufferId
      );

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw RuntimeErrorException("Failed to create framebuffer");
    }
    catch (...)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
      glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);

      destroy();
      throw;
    }

    m_width = width;
    m_height = height;
    m_valid = true;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
  }

  void OpenGlFrameBuffer::bind()
  {
    assertValid();
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
  }

  void OpenGlFrameBuffer::bindForReadingOnly()
  {
    assertValid();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferId);
  }

  void OpenGlFrameBuffer::bindForDrawingOnly()
  {
    assertValid();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBufferId);
  }

  void OpenGlFrameBuffer::unbind()
  {
    assertValid();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGlFrameBuffer::resize(UInt32 width, UInt32 height)
  {
    assertValid();

    if (width == m_width && height == m_height)
      return;

    if (width == 0 || height == 0)
      throw InvalidArgumentException("Framebuffer dimensions must be greater than zero");

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    GLint currentRenderbuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRenderbuffer);

    try
    {
      m_colorTexture.resize(width, height);

      glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBufferId);
      glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
        width, height
      );

      glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      {
        m_valid = false;
        throw RuntimeErrorException("Failed to resize framebuffer");
      }
    }
    catch (...)
    {
      glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
      throw;
    }

    m_width = width;
    m_height = height;

    glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
  }

  ITexture& OpenGlFrameBuffer::getColorTexture()
  {
    return m_colorTexture;
  }

  UInt32 OpenGlFrameBuffer::getWidth() const
  {
    return m_width;
  }

  UInt32 OpenGlFrameBuffer::getHeight() const
  {
    return m_height;
  }

  void OpenGlFrameBuffer::clear(const Color& clearColor)
  {
    assertValid();

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    try
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
      glClearBufferfv(GL_COLOR, 0, clearColor.m);
      glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

  bool OpenGlFrameBuffer::isValid() const
  {
    return m_valid;
  }

  void OpenGlFrameBuffer::destroy()
  {
    if (m_depthStencilBufferId != 0)
    {
      GLint currentRenderbuffer = 0;
      glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRenderbuffer);
      if (currentRenderbuffer == static_cast<GLint>(m_depthStencilBufferId))
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

      glDeleteRenderbuffers(1, &m_depthStencilBufferId);
      m_depthStencilBufferId = 0;
    }

    if (m_frameBufferId != 0)
    {
      GLint currentReadFramebuffer = 0;
      glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFramebuffer);
      if (currentReadFramebuffer == static_cast<GLint>(m_frameBufferId))
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      
      GLint currentDrawFramebuffer = 0;
      glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFramebuffer);
      if (currentDrawFramebuffer == static_cast<GLint>(m_frameBufferId))
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

      glDeleteFramebuffers(1, &m_frameBufferId);
      m_frameBufferId = 0;
    }

    m_colorTexture.destroy();
    m_width = 0;
    m_height = 0;
    m_valid = false;
  }

  void OpenGlFrameBuffer::copyDepthTo(IFrameBuffer& destinationFrameBuffer)
  {
    assertValid();
    if (!destinationFrameBuffer.isValid())
      throw InvalidArgumentException("Destination framebuffer is not valid");

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

  void OpenGlFrameBuffer::destroySelf()
  {
    delete this;
  }

  void OpenGlFrameBuffer::assertValid() const
  {
    if (!m_valid)
      throw RuntimeErrorException("Framebuffer is not initialized or is invalid");
  }
}
