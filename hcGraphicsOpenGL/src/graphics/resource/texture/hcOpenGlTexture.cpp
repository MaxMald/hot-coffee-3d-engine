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
    const TextureInitializationSettings& initializationSettings,
    const Path& sourcePath
  )
  {
    if (image.getWidth() != initializationSettings.width || image.getHeight() != initializationSettings.height)
      throw InvalidArgumentException(
        String::Format(
          "Image dimensions (%u x %u) do not match the specified initialization settings (%u x %u).",
          image.getWidth(),
          image.getHeight(),
          initializationSettings.width,
          initializationSettings.height
        )
      );

    if (image.getColorSpace() != initializationSettings.colorSpace)
      throw InvalidArgumentException(
        String::Format(
          "Image color space (%u) does not match the specified initialization settings (%u).",
          static_cast<UInt32>(image.getColorSpace()),
          static_cast<UInt32>(initializationSettings.colorSpace)
        )
      );

    if (image.getFormat() != initializationSettings.format)
      throw InvalidArgumentException(
        String::Format(
          "Image format (%u) does not match the specified initialization settings (%u).",
          static_cast<UInt32>(image.getFormat()),
          static_cast<UInt32>(initializationSettings.format)
        )
      );

    initialize(
      initializationSettings,
      static_cast<const void*>(image.getBuffer().data()),
      sourcePath
    );
  }

  void OpenGlTexture::initialize(
    const TextureInitializationSettings& initializationSettings
  )
  {
    initialize(initializationSettings, nullptr, Path());
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

  void OpenGlTexture::initialize(
    const TextureInitializationSettings& settings,
    const void* pixelData,
    const Path& sourcePath
  )
  {
    if (m_created)
      throw RuntimeErrorException("Texture has already been created, cannot re-initialize.");

    if (settings.width == 0 || settings.height == 0)
      throw InvalidArgumentException(
        String::Format(
          "Invalid image dimensions (%u x %u) for texture creation. Dimensions must be greater than zero.",
          settings.width,
          settings.height
        )
      );

    GLint currentTextureId = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId);

    try
    {
      glGenTextures(1, &m_textureId);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glBindTexture(GL_TEXTURE_2D, m_textureId);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, settings.mipmapMaxLevel);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, openGlGraphicsUtilities::GetOpenGlWrapMode(settings.wrap));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, openGlGraphicsUtilities::GetOpenGlWrapMode(settings.wrap));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, openGlGraphicsUtilities::GetOpenGlMinFilter(settings.minFilter, settings.mipmapMode));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, openGlGraphicsUtilities::GetOpenGlMagFilter(settings.magFilter));

      GLint glInternalFormat = openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(settings.format, settings.colorSpace);
      GLenum glFormat = openGlGraphicsUtilities::GetOpenGlFormatFromTextureFormatType(settings.format);
      GLenum glType = openGlGraphicsUtilities::GetOpenGLDataTypeFromTextureFormatType(settings.format);

      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        glInternalFormat,
        static_cast<GLsizei>(settings.width), static_cast<GLsizei>(settings.height),
        0,
        glFormat,
        glType,
        pixelData
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

    m_width = settings.width;
    m_height = settings.height;
    m_colorSpace = settings.colorSpace;
    m_textureFormat = settings.format;
    m_sourcePath = sourcePath;
    m_created = true;
  }
}
