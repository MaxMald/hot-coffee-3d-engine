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
    m_positionTexture(),
    m_normalRoughnessTexture(),
    m_albedoAlphaTexture(),
    m_materialParametersTexture()
  {}

  OpenGlGBuffer::~OpenGlGBuffer()
  {}

  void OpenGlGBuffer::initialize(UInt32 width, UInt32 height)
  {
    if (m_valid)
      throw RuntimeErrorException("GBuffer is already initialized.");

    if (width == 0 || height == 0)
      throw RuntimeErrorException("GBuffer dimensions must be greater than zero.");

    glGenFramebuffers(1, &m_gBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);

    m_positionTexture.initialize(
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

    glBindTexture(GL_TEXTURE_2D, m_positionTexture.getTextureId());
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
      m_positionTexture.getTextureId(), 0
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

  void OpenGlGBuffer::bindForWriting()
  {
    assertIsValid();

    glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);
  }

  void OpenGlGBuffer::bindForReading()
  {
    assertIsValid();

    m_positionTexture.bind(0);
    m_normalRoughnessTexture.bind(1);
    m_albedoAlphaTexture.bind(2);
    m_materialParametersTexture.bind(3);
  }

  void OpenGlGBuffer::clear()
  {
    assertIsValid();

    glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGlGBuffer::unbind()
  {
    assertIsValid();

    m_positionTexture.unbind(0);
    m_normalRoughnessTexture.unbind(1);
    m_albedoAlphaTexture.unbind(2);
    m_materialParametersTexture.unbind(3);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  bool OpenGlGBuffer::isValid() const
  {
    return m_valid;
  }

  UInt32 OpenGlGBuffer::getWidth() const
  {
    return m_width;
  }

  UInt32 OpenGlGBuffer::getHeight() const
  {
    return m_height;
  }

  void OpenGlGBuffer::resize(UInt32 width, UInt32 height)
  {
    assertIsValid();

    if (width == m_width && height == m_height)
      return;

    if (width == 0 || height == 0)
      throw RuntimeErrorException("GBuffer dimensions must be greater than zero.");

    m_positionTexture.resize(width, height);
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

  const ITexture& OpenGlGBuffer::getPosition() const
  {
    return m_positionTexture;
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
}
