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
    cleanup();
  }

  void OpenGlGBuffer::initialize(UInt32 width, UInt32 height)
  {
    if (m_valid)
      throw RuntimeErrorException("GBuffer is already initialized.");

    if (width == 0 || height == 0)
      throw InvalidArgumentException("GBuffer dimensions must be greater than zero.");

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    GLint currentRenderbuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRenderbuffer);

    try
    {
      glGenFramebuffers(1, &m_gBufferId);
      glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);

      m_positionAndDepthTexture.initialize(
        width, height,
        GL_RGBA16F, GL_RGBA, GL_FLOAT
      );

      if (!m_positionAndDepthTexture.isValid())
        throw RuntimeErrorException("Failed to create position and depth texture for GBuffer.");

      m_normalRoughnessTexture.initialize(
        width, height,
        GL_RGBA16F, GL_RGBA, GL_FLOAT
      );

      if (!m_normalRoughnessTexture.isValid())
        throw RuntimeErrorException("Failed to create normal and roughness texture for GBuffer.");

      m_albedoAlphaTexture.initialize(
        width, height,
        GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE
      );

      if (!m_albedoAlphaTexture.isValid())
        throw RuntimeErrorException("Failed to create albedo and alpha texture for GBuffer.");

      m_materialParametersTexture.initialize(
        width, height,
        GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE
      );

      if (!m_materialParametersTexture.isValid())
        throw RuntimeErrorException("Failed to create material parameters texture for GBuffer.");

      glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        m_positionAndDepthTexture.getTextureId(), 0
      );

      glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
        m_normalRoughnessTexture.getTextureId(), 0
      );

      glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
        m_albedoAlphaTexture.getTextureId(), 0
      );

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
        throw RuntimeErrorException("Failed to initialize GBuffer: Incomplete framebuffer.");
    }
    catch (...)
    {
      glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);

      cleanup();
      throw;
    }

    m_width = width;
    m_height = height;
    m_valid = true;

    glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
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

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    GLint currentRenderbuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRenderbuffer);

    try
    {
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
        m_valid = false;
        throw RuntimeErrorException("Failed to resize GBuffer.");
      }
    }
    catch(...)
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

  void OpenGlGBuffer::clear(const Color&)
  {
    assertIsValid();

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    try
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);
      glClearBufferfv(GL_COLOR, 0, IGBuffer::CLEAR_COLOR_POSITION_AND_DEPTH);
      glClearBufferfv(GL_COLOR, 1, IGBuffer::CLEAR_COLOR_NORMAL_AND_ROUGHNESS);
      glClearBufferfv(GL_COLOR, 2, IGBuffer::CLEAR_COLOR_ALBEDO_AND_ALPHA);
      glClearBufferfv(GL_COLOR, 3, IGBuffer::CLEAR_COLOR_MATERIAL_PARAMETERS);
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

  bool OpenGlGBuffer::isValid() const
  {
    return m_valid;
  }

  void OpenGlGBuffer::cleanup()
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

    if (m_gBufferId != 0)
    {
      GLint currentReadFramebuffer = 0;
      glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFramebuffer);
      if (currentReadFramebuffer == static_cast<GLint>(m_gBufferId))
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

      GLint currentDrawFramebuffer = 0;
      glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFramebuffer);
      if (currentDrawFramebuffer == static_cast<GLint>(m_gBufferId))
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

      glDeleteFramebuffers(1, &m_gBufferId);
      m_gBufferId = 0;
    }

    m_positionAndDepthTexture.destroy();
    m_normalRoughnessTexture.destroy();
    m_albedoAlphaTexture.destroy();
    m_materialParametersTexture.destroy();

    m_width = 0;
    m_height = 0;
    m_valid = false;
  }

  void OpenGlGBuffer::copyDepthTo(IFrameBuffer& destinationFrameBuffer)
  {
    assertIsValid();
    if (!destinationFrameBuffer.isValid())
      throw InvalidArgumentException("Destination framebuffer is not valid.");

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
    delete this;
  }
}
