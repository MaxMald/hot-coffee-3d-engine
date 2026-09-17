#include "hc/graphics/resource/frameBuffer/hcOpenGlShadowFrameBuffer.h"
#include <GL/glew.h>

namespace hc
{
  OpenGlShadowFrameBuffer::OpenGlShadowFrameBuffer() :
    m_frameBufferId(0),
    m_depthTextureArrayId(0),
    m_width(0),
    m_height(0),
    m_numLayers(0),
    m_initialized(false)
  {}

  OpenGlShadowFrameBuffer::~OpenGlShadowFrameBuffer()
  {
    destroy();
  }

  void OpenGlShadowFrameBuffer::initialize(UInt32 width, UInt32 height, UInt32 numLayers)
  {
    if (m_initialized)
      throw RuntimeErrorException("OpenGlShadowFrameBuffer is already initialized.");

    if (width == 0 || height == 0 || numLayers == 0)
      throw RuntimeErrorException("OpenGlShadowFrameBuffer: width, height, and number of layers must be greater than zero.");

    GLint currentTextureArrayId = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &currentTextureArrayId);
    GLint currentFrameBufferId = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFrameBufferId);

    try
    {
      glGenTextures(1, &m_depthTextureArrayId);
      glBindTexture(GL_TEXTURE_2D_ARRAY, m_depthTextureArrayId);
      glTexStorage3D(
        GL_TEXTURE_2D_ARRAY,
        1,
        GL_DEPTH_COMPONENT32F,
        width, height,
        numLayers
      );

      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glGenFramebuffers(1, &m_frameBufferId);
      glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

      glFramebufferTextureLayer(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        m_depthTextureArrayId,
        0, // mipmap level
        0  // default target layer is 0
      );

      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw RuntimeErrorException("Failed to initialize OpenGlShadowFrameBuffer: Incomplete framebuffer.");
    }
    catch (...)
    {
      glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<GLuint>(currentTextureArrayId));
      glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(currentFrameBufferId));
      destroy();
      throw;
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<GLuint>(currentTextureArrayId));
    glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(currentFrameBufferId));

    m_width = width;
    m_height = height;
    m_numLayers = numLayers;
    m_initialized = true;
  }

  UInt32 OpenGlShadowFrameBuffer::getWidth() const
  {
    return m_width;
  }

  UInt32 OpenGlShadowFrameBuffer::getHeight() const
  {
    return m_height;
  }

  UInt32 OpenGlShadowFrameBuffer::getNumLayers() const
  {
    return m_numLayers;
  }

  void OpenGlShadowFrameBuffer::bind(UInt32 layer)
  {
    assertIsInitialized();

    if (layer >= m_numLayers)
      throw InvalidArgumentException("OpenGlShadowFrameBuffer: layer index is out of bounds.");

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    glFramebufferTextureLayer(
      GL_FRAMEBUFFER,
      GL_DEPTH_ATTACHMENT,
      m_depthTextureArrayId,
      0, // mipmap level
      layer
    );
    glClear(GL_DEPTH_BUFFER_BIT);
  }

  void OpenGlShadowFrameBuffer::unbind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  UInt32 OpenGlShadowFrameBuffer::getTextureArrayId() const
  {
    return m_depthTextureArrayId;
  }

  void OpenGlShadowFrameBuffer::destroy()
  {
    if (m_frameBufferId != 0)
    {
      GLint currentFrameBufferId = 0;
      glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFrameBufferId);

      if (currentFrameBufferId == static_cast<GLint>(m_frameBufferId))
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

      glDeleteFramebuffers(1, &m_frameBufferId);
      m_frameBufferId = 0;
    }

    if (m_depthTextureArrayId != 0)
    {
      GLint currentTextureArrayId = 0;
      glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &currentTextureArrayId);

      if (currentTextureArrayId == static_cast<GLint>(m_depthTextureArrayId))
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

      glDeleteTextures(1, &m_depthTextureArrayId);
      m_depthTextureArrayId = 0;
    }

    m_width = 0;
    m_height = 0;
    m_numLayers = 0;
    m_initialized = false;
  }

  void OpenGlShadowFrameBuffer::assertIsInitialized() const
  {
    if (!m_initialized)
      throw RuntimeErrorException("OpenGlShadowFrameBuffer is not initialized.");
  }
}
