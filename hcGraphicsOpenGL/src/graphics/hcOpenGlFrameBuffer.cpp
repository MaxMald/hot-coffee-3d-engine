#include "hc/graphics/hcOpenGlFrameBuffer.h"
#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

namespace hc
{
  OpenGlFrameBuffer::OpenGlFrameBuffer() :
    m_width(0),
    m_height(0),
    m_frameBufferId(0),
    m_depthStencilBufferId(0),
    m_colorTexture(nullptr),
    m_isValid(false),
    m_isBound(false),
    m_previousViewport{ 0, 0, 0, 0 }
  {
  }

  OpenGlFrameBuffer::~OpenGlFrameBuffer()
  {
    cleanup();
  }

  void OpenGlFrameBuffer::initialize(UInt32 width, UInt32 height)
  {
    try
    {
      if (m_isValid)
        throw RuntimeErrorException("Framebuffer is already initialized");

      if (width == 0 || height == 0)
        throw InvalidArgumentException(
          "Framebuffer dimensions must be greater than zero"
        );

      m_colorTexture = MakeUnique<OpenGlTexture>(width, height);
      if (!m_colorTexture->isValid())
        throw RuntimeErrorException("Failed to create color texture for framebuffer");

      glGenFramebuffers(1, &m_frameBufferId);
      glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

      glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        m_colorTexture->getTextureId(),
        0
      );

      glGenRenderbuffers(1, &m_depthStencilBufferId);
      glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBufferId);
      glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        width,
        height
      );

      glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        m_depthStencilBufferId
      );

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        throw RuntimeErrorException("Failed to create framebuffer");
      }

      m_width = width;
      m_height = height;
      m_isValid = true;

      glBindTexture(GL_TEXTURE_2D, 0);
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    catch (const Exception&)
    {
      cleanup();
      throw;
    }
  }

  void OpenGlFrameBuffer::bind()
  {
    assertValid();

    if (m_isBound)
      return;

    savePreviousViewport();
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    glViewport(0, 0, m_width, m_height);
    m_isBound = true;
  }

  void OpenGlFrameBuffer::unbind()
  {
    assertValid();

    if (!m_isBound)
      return;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    restorePreviousViewport();
    m_isBound = false;
  }

  void OpenGlFrameBuffer::resize(UInt32 width, UInt32 height)
  {
    assertValid();

    if (width == m_width && height == m_height)
      return;

    if (width == 0 || height == 0)
      throw InvalidArgumentException("Framebuffer dimensions must be greater than zero");

    m_colorTexture->resize(width, height);

    glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBufferId);
    glRenderbufferStorage(
      GL_RENDERBUFFER,
      GL_DEPTH24_STENCIL8,
      width,
      height
    );

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      m_isValid = false;
      throw RuntimeErrorException("Failed to resize framebuffer");
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_width = width;
    m_height = height;
  }

  ITexture& OpenGlFrameBuffer::getColorTexture()
  {
    return *m_colorTexture;
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

    if (!m_isBound)
      throw RuntimeErrorException("Framebuffer must be bound before clearing");

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  bool OpenGlFrameBuffer::isValid() const
  {
    return m_isValid;
  }

  void OpenGlFrameBuffer::cleanup()
  {
    if (m_isBound)
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      restorePreviousViewport();
      m_isBound = false;
    }

    if (m_depthStencilBufferId != 0)
    {
      glDeleteRenderbuffers(1, &m_depthStencilBufferId);
      m_depthStencilBufferId = 0;
    }

    if (m_frameBufferId != 0)
    {
      glDeleteFramebuffers(1, &m_frameBufferId);
      m_frameBufferId = 0;
    }

    m_colorTexture.reset();
    m_isValid = false;
  }

  void OpenGlFrameBuffer::destroy()
  {
    cleanup();
    delete this;
  }

  void OpenGlFrameBuffer::savePreviousViewport()
  {
    glGetIntegerv(GL_VIEWPORT, m_previousViewport);
  }

  void OpenGlFrameBuffer::restorePreviousViewport()
  {
    if (m_isBound)
    {
      glViewport(m_previousViewport[0], m_previousViewport[1],
        m_previousViewport[2], m_previousViewport[3]);
    }
  }

  void OpenGlFrameBuffer::assertValid() const
  {
    if (!m_isValid)
      throw RuntimeErrorException("Framebuffer is not initialized or is invalid");
  }
}
