#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  OpenGlTexture::OpenGlTexture() :
    m_sourcePath(),
    m_textureId(0),
    m_width(0),
    m_height(0),
    m_textureFormat(textureFormatType::RGBA8),
    m_colorSpace(colorSpaceType::Linear),
    m_created(false)
  {}

  OpenGlTexture::~OpenGlTexture()
  {
    destroy();
  }

  void OpenGlTexture::initialize(
    const Image& image,
    const Path& sourcePath
  )
  {
    if (m_created)
      throw RuntimeErrorException("Texture has already been created, cannot re-initialize.");

    UInt32 width = image.getWidth();
    UInt32 height = image.getHeight();
    if (width == 0 || height == 0)
      throw InvalidArgumentException(
        String::Format(
          "Invalid image dimensions (%u x %u) for texture creation. Dimensions must be greater than zero.",
          width,
          height
        )
      );

    GLint currentTextureId = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId);

    try
    {
      glGenTextures(1, &m_textureId);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glBindTexture(GL_TEXTURE_2D, m_textureId);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      GLint glInternalFormat = openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(image.getFormat(), image.getColorSpace());
      GLenum glFormat = openGlGraphicsUtilities::GetOpenGlFormatFromTextureFormatType(image.getFormat());
      GLenum glType = openGlGraphicsUtilities::GetOpenGLDataTypeFromTextureFormatType(image.getFormat());

      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        glInternalFormat,
        static_cast<GLsizei>(width), static_cast<GLsizei>(height),
        0,
        glFormat,
        glType,
        image.getBuffer().data()
      );

      openGlGraphicsUtilities::AssertOpenGlHasNoError();
    }
    catch (...)
    {
      if (m_textureId)
      {
        glDeleteTextures(1, &m_textureId);
        m_textureId = 0;
      }

      glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(currentTextureId));
      throw;
    }

    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(currentTextureId));

    m_width = width;
    m_height = height;
    m_colorSpace = image.getColorSpace();
    m_textureFormat = image.getFormat();
    m_sourcePath = sourcePath;
    m_created = true;
  }

  UInt32 OpenGlTexture::getWidth() const
  {
    return m_width;
  }

  UInt32 OpenGlTexture::getHeight() const
  {
    return m_height;
  }

  textureFormatType::Type OpenGlTexture::getTextureFormat() const
  {
    return m_textureFormat;
  }

  colorSpaceType::Type OpenGlTexture::getColorSpace() const
  {
    return m_colorSpace;
  }

  void OpenGlTexture::resize(UInt32 width, UInt32 height)
  {
    assertIsCreated();

    if (width == m_width && height == m_height)
      return;

    if (width == 0 || height == 0)
      throw InvalidArgumentException(
        String::Format(
          "Invalid image dimensions (%u x %u) for texture creation. Dimensions must be greater than zero.",
          width,
          height
        )
      );

    GLint currentTextureId = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId);

    try
    {
      glBindTexture(GL_TEXTURE_2D, m_textureId);
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(m_textureFormat, m_colorSpace)),
        static_cast<Int32>(width), static_cast<Int32>(height),
        0,
        static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGlFormatFromTextureFormatType(m_textureFormat)),
        static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGLDataTypeFromTextureFormatType(m_textureFormat)),
        nullptr
      );

      openGlGraphicsUtilities::AssertOpenGlHasNoError();
    }
    catch (...)
    {
      glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(currentTextureId));
      throw;
    }

    m_width = width;
    m_height = height;

    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(currentTextureId));
  }

  void OpenGlTexture::bind(UInt32 slot) const
  {
    assertIsCreated();

    GLint currentActiveTextureId = 0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTextureId);

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glActiveTexture(static_cast<GLuint>(currentActiveTextureId));
  }

  void OpenGlTexture::unbind(UInt32 slot) const
  {
    if (!m_created)
      return;

    GLint currentActiveTextureId = 0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTextureId);

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(static_cast<GLuint>(currentActiveTextureId));
  }

  bool OpenGlTexture::isValid() const
  {
    return m_created;
  }

  void OpenGlTexture::destroy()
  {
    if (m_textureId)
    {
      GLint currentTextureId = 0;
      glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId);
      if (currentTextureId == static_cast<GLint>(m_textureId))
        glBindTexture(GL_TEXTURE_2D, 0);

      glDeleteTextures(1, &m_textureId);
      m_textureId = 0;
    }

    m_width = 0;
    m_height = 0;
    m_textureFormat = textureFormatType::RGBA8;
    m_colorSpace = colorSpaceType::Linear;
    m_created = false;
  }

  void* OpenGlTexture::getNativeHandle() const
  {
    // Return the address of the GLuint as a void* for interoperability
    return reinterpret_cast<void*>(static_cast<uintptr_t>(m_textureId));
  }

  GLuint OpenGlTexture::getTextureId() const
  {
    return m_textureId;
  }
}
