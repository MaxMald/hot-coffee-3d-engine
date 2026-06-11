#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  OpenGlTexture::OpenGlTexture() :
    m_id(Id::Create()),
    m_textureId(0),
    m_width(0),
    m_height(0),
    m_channels(0),
    m_internalFormat(GL_RGBA8),
    m_format(GL_RGBA),
    m_type(GL_UNSIGNED_BYTE),
    m_created(false)
  {}

  OpenGlTexture::OpenGlTexture(SharedPtr<Image> image) :
    m_id(Id::Create()),
    m_textureId(0),
    m_width(0),
    m_height(0),
    m_channels(0),
    m_internalFormat(GL_RGBA8),
    m_format(GL_RGBA),
    m_type(GL_UNSIGNED_BYTE),
    m_created(false)
  {
    if (!image)
      return;

    initialize(*image);
  }

  OpenGlTexture::OpenGlTexture(
    UInt32 width,
    UInt32 height,
    GLenum internalFormat,
    GLenum format,
    GLenum type
  ) :
    m_id(Id::Create()),
    m_textureId(0),
    m_width(width),
    m_height(height),
    m_channels(0),
    m_internalFormat(internalFormat),
    m_format(format),
    m_type(type),
    m_created(false)
  {
    initialize(width, height, internalFormat, format, type);
  }

  OpenGlTexture::~OpenGlTexture()
  {
    destroy();
  }

  const Id& OpenGlTexture::getId() const
  {
    return m_id;
  }

  void OpenGlTexture::initialize(const Image& image)
  {
    if (m_created)
      throw RuntimeErrorException("Texture has already been created, cannot re-initialize.");

    GLenum format = (image.getChannels() == 4) ? GL_RGBA : GL_RGB;
    GLenum internalFormat = (image.getChannels() == 4) ? GL_RGBA8 : GL_RGB8;

    initialize(
      image.getWidth(),
      image.getHeight(),
      internalFormat,
      format,
      GL_UNSIGNED_BYTE,
      image.getBuffer().data()
    );
  }

  void OpenGlTexture::initialize(UInt32 width, UInt32 height, colorFormatType::Type format)
  {
    if (m_created)
      throw RuntimeErrorException("Texture has already been created, cannot re-initialize.");

    assertDimensionsAreGreaterThanZero(width, height);

    initialize(
      width,
      height,
      static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGlInternalFormatFromColorFormatType(format)),
      static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGlFormatFromColorFormatType(format)),
      GL_UNSIGNED_BYTE
    );
  }

  void OpenGlTexture::initialize(
    UInt32 width,
    UInt32 height,
    colorFormatType::Type format,
    const Color& initColor
  )
  {
    if (m_created)
      throw RuntimeErrorException("Texture has already been created, cannot re-initialize.");

    assertDimensionsAreGreaterThanZero(width, height);

    UInt8 channels = colorFormatType::GetChannelCount(format);
    BufferByte initData(width * height * static_cast<SizeT>(channels));
    Byte r = static_cast<Byte>(initColor.r * 255);
    Byte g = static_cast<Byte>(initColor.g * 255);
    Byte b = static_cast<Byte>(initColor.b * 255);
    Byte a = static_cast<Byte>(initColor.a * 255);

    for (size_t i = 0; i < width * height; ++i)
    {
      initData[i * channels + 0] = r;
      initData[i * channels + 1] = g;
      initData[i * channels + 2] = b;
      if (channels == 4)
        initData[i * channels + 3] = a;
    }

    initialize(
      width,
      height,
      static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGlInternalFormatFromColorFormatType(format)),
      static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGlFormatFromColorFormatType(format)),
      GL_UNSIGNED_BYTE,
      initData.data()
    );
  }

  UInt32 OpenGlTexture::getWidth() const
  {
    return m_width;
  }

  UInt32 OpenGlTexture::getHeight() const
  {
    return m_height;
  }

  UInt8 OpenGlTexture::getChannels() const
  {
    return m_channels;
  }

  void OpenGlTexture::resize(UInt32 width, UInt32 height)
  {
    assertIsCreated();

    if (width == m_width && height == m_height)
      return;

    if (width == 0 || height == 0)
      throw InvalidArgumentException("Texture dimensions must be greater than zero");

    GLint currentTextureId = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId);

    try
    {
      glBindTexture(GL_TEXTURE_2D, m_textureId);
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        m_internalFormat,
        static_cast<Int32>(width), static_cast<Int32>(height),
        0, m_format, m_type,
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
    m_channels = 0;
    m_internalFormat = GL_RGBA8;
    m_format = GL_RGBA;
    m_type = GL_UNSIGNED_BYTE;
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
    UInt32 width,
    UInt32 height,
    GLenum internalFormat,
    GLenum format,
    GLenum type,
    const void* initData
  )
  {
    if (m_created)
      throw RuntimeErrorException("Texture has already been created, cannot re-initialize.");

    assertDimensionsAreGreaterThanZero(width, height);

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
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        static_cast<Int32>(width), static_cast<Int32>(height),
        0, format, type,
        initData
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

    m_width = width;
    m_height = height;
    m_channels = openGlGraphicsUtilities::GetChannelCountFromOpenGlFormat(format);
    m_format = format;
    m_internalFormat = internalFormat;
    m_type = type;
    m_created = true;

    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(currentTextureId));
  }

  void OpenGlTexture::assertDimensionsAreGreaterThanZero(UInt32 width, UInt32 height)
  {
    if (width == 0 || height == 0)
      throw InvalidArgumentException(
        String::Format(
          "Invalid image dimensions (%u x %u) for texture creation. Dimensions must be greater than zero.",
          width,
          height
        )
      );
  }

  void OpenGlTexture::assertIsCreated() const
  {
    if (!m_created)
      throw RuntimeErrorException("Texture has not been created yet.");
  }
}
