#include "hc/graphics/hcOpenGlFrameBuffer.h"
#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

namespace hc
{
  OpenGlFrameBuffer::OpenGlFrameBuffer(
    UInt32 width,
    UInt32 height
  ) :
    m_width(width),
    m_height(height),
    m_frameBufferId(0),
    m_depthStencilBufferId(0),
    m_colorTexture(nullptr),
    m_isValid(false)
  {
    if (m_width == 0 || m_height == 0)
      throw InvalidArgumentException(
        "Framebuffer dimensions must be greater than zero"
      );
    
    m_colorTexture = MakeUnique<OpenGlTexture>(m_width, m_height);
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
      m_width,
      m_height
    );

    glFramebufferRenderbuffer(
      GL_FRAMEBUFFER,
      GL_DEPTH_STENCIL_ATTACHMENT,
      GL_RENDERBUFFER,
      m_depthStencilBufferId
    );


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      throw RuntimeErrorException("Failed to create framebuffer");
    }

    m_isValid = true;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  OpenGlFrameBuffer::~OpenGlFrameBuffer()
  {
    destroy();
  }

  void OpenGlFrameBuffer::bind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    glViewport(0, 0, m_width, m_height);
  }

  void OpenGlFrameBuffer::unbind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGlFrameBuffer::resize(UInt32 width, UInt32 height)
  {
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
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  bool OpenGlFrameBuffer::isValid() const
  {
    return m_isValid;
  }

  void OpenGlFrameBuffer::destroy()
  {
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
}
