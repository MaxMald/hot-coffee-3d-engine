#include "hc/graphics/hcOpenGlGBuffer.h"

#include <GL/glew.h>
#include <hc/graphics/resource/texture/hcITexture.h>

namespace hc
{
  inline static constexpr GLenum GBufferColorAttachments[4] = {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3
  };

  OpenGlGBuffer::OpenGlGBuffer() :
    m_valid(false),
    m_width(0),
    m_height(0),
    m_gBufferId(0),
    m_depthStencilBufferId(0),
    m_positionAndDepthTexture(),
    m_normalRoughnessTexture(),
    m_albedoAlphaTexture(),
    m_materialParametersTexture()
  {}

  OpenGlGBuffer::~OpenGlGBuffer()
  {
    destroy();
  }

  void OpenGlGBuffer::initialize(UInt32 width, UInt32 height)
  {
    try
    {
      if (m_valid)
        throw RuntimeErrorException("GBuffer is already initialized.");

      if (width == 0 || height == 0)
        throw InvalidArgumentException("GBuffer dimensions must be greater than zero.");

      glGenFramebuffers(1, &m_gBufferId);
      glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);

      m_positionAndDepthTexture.initialize(
        width, height,
        GL_RGBA16F, GL_RGBA, GL_FLOAT
      );

      m_normalRoughnessTexture.initialize(
        width, height,
        GL_RGBA16F, GL_RGBA, GL_FLOAT
      );

      m_albedoAlphaTexture.initialize(
        width, height,
        GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE
      );

      m_materialParametersTexture.initialize(
        width, height,
        GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE
      );

      glBindTexture(GL_TEXTURE_2D, m_positionAndDepthTexture.getTextureId());
      glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        m_positionAndDepthTexture.getTextureId(), 0
      );

      glBindTexture(GL_TEXTURE_2D, m_normalRoughnessTexture.getTextureId());
      glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
        m_normalRoughnessTexture.getTextureId(), 0
      );

      glBindTexture(GL_TEXTURE_2D, m_albedoAlphaTexture.getTextureId());
      glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
        m_albedoAlphaTexture.getTextureId(), 0
      );

      glBindTexture(GL_TEXTURE_2D, m_materialParametersTexture.getTextureId());
      glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D,
        m_materialParametersTexture.getTextureId(), 0
      );

      glDrawBuffers(4, GBufferColorAttachments);

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
        throw RuntimeErrorException("Failed to initialize GBuffer: Incomplete framebuffer.");
      }

      glBindTexture(GL_TEXTURE_2D, 0);
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      m_width = width;
      m_height = height;
      m_valid = true;
    }
    catch (const Exception& e)
    {
      destroy();
      throw;
    }
  }

  void OpenGlGBuffer::bind()
  {
    assertIsValid();
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);
  }

  void OpenGlGBuffer::bindForReadingOnly()
  {
    assertIsValid();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBufferId);
  }

  void OpenGlGBuffer::bindForDrawingOnly()
  {
    assertIsValid();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_gBufferId);
  }

  void OpenGlGBuffer::unbind()
  {
    assertIsValid();

    m_positionAndDepthTexture.unbind(0);
    m_normalRoughnessTexture.unbind(1);
    m_albedoAlphaTexture.unbind(2);
    m_materialParametersTexture.unbind(3);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGlGBuffer::resize(UInt32 width, UInt32 height)
  {
    assertIsValid();

    if (width == m_width && height == m_height)
      return;

    if (width == 0 || height == 0)
      throw RuntimeErrorException("GBuffer dimensions must be greater than zero.");

    m_positionAndDepthTexture.resize(width, height);
    m_normalRoughnessTexture.resize(width, height);
    m_albedoAlphaTexture.resize(width, height);
    m_materialParametersTexture.resize(width, height);

    glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBufferId);
    glRenderbufferStorage(
      GL_RENDERBUFFER,
      GL_DEPTH24_STENCIL8,
      width,
      height
    );

    glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      m_valid = false;

      throw RuntimeErrorException("Failed to resize framebuffer");
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_width = width;
    m_height = height;
  }

  ITexture& OpenGlGBuffer::getColorTexture()
  {
    return m_albedoAlphaTexture;
  }

  UInt32 OpenGlGBuffer::getWidth() const
  {
    return m_width;
  }

  UInt32 OpenGlGBuffer::getHeight() const
  {
    return m_height;
  }

  void OpenGlGBuffer::clear(const Color& clearColor)
  {
    assertIsValid();

    glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);

    glClearBufferfv(GL_COLOR, 0, IGBuffer::CLEAR_COLOR_POSITION_AND_DEPTH);
    glClearBufferfv(GL_COLOR, 1, IGBuffer::CLEAR_COLOR_NORMAL_AND_ROUGHNESS);
    glClearBufferfv(GL_COLOR, 2, IGBuffer::CLEAR_COLOR_ALBEDO_AND_ALPHA);
    glClearBufferfv(GL_COLOR, 3, IGBuffer::CLEAR_COLOR_MATERIAL_PARAMETERS);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  bool OpenGlGBuffer::isValid() const
  {
    return m_valid;
  }

  void OpenGlGBuffer::cleanup()
  {
    destroy();
  }

  void OpenGlGBuffer::copyDepthTo(IFrameBuffer& destinationFrameBuffer)
  {
    assertIsValid();

    bindForReadingOnly();
    destinationFrameBuffer.bindForDrawingOnly();

    glBlitFramebuffer(
      0, 0, m_width, m_height,
      0, 0, destinationFrameBuffer.getWidth(), destinationFrameBuffer.getHeight(),
      GL_DEPTH_BUFFER_BIT,
      GL_NEAREST
    );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGlGBuffer::bindGTexturesForReading()
  {
    assertIsValid();
    m_positionAndDepthTexture.bind(0);
    m_normalRoughnessTexture.bind(1);
    m_albedoAlphaTexture.bind(2);
    m_materialParametersTexture.bind(3);
  }

  const ITexture& OpenGlGBuffer::getPositionAndDepth() const
  {
    return m_positionAndDepthTexture;
  }

  const ITexture& OpenGlGBuffer::getAlbedoAlpha() const
  {
    return m_albedoAlphaTexture;
  }

  const ITexture& OpenGlGBuffer::getNormalRoughness() const
  {
    return m_normalRoughnessTexture;
  }

  const ITexture& OpenGlGBuffer::getMaterialParameters() const
  {
    return m_materialParametersTexture;
  }

  void OpenGlGBuffer::assertIsValid() const
  {
    if (!m_valid)
      throw RuntimeErrorException("GBuffer is not valid.");
  }

  void OpenGlGBuffer::destroy()
  {
    m_positionAndDepthTexture.destroy();
    m_normalRoughnessTexture.destroy();
    m_albedoAlphaTexture.destroy();
    m_materialParametersTexture.destroy();

    if (m_depthStencilBufferId != 0)
    {
      glDeleteRenderbuffers(1, &m_depthStencilBufferId);
      m_depthStencilBufferId = 0;
    }

    if (m_gBufferId != 0)
    {
      glDeleteFramebuffers(1, &m_gBufferId);
      m_gBufferId = 0;
    }

    m_width = 0;
    m_height = 0;
    m_valid = false;
  }
}
