#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

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

    if (image.getWidth() == 0 || image.getHeight() == 0)
      throw InvalidArgumentException(
        String::Format(
          "Invalid image dimensions (%ux%u) for texture creation. Dimensions must be greater than zero.",
          image.getWidth(),
          image.getHeight()
        )
      );

    if (image.getChannels() != 3 && image.getChannels() != 4)
      throw InvalidArgumentException(
        String::Format(
          "Unsupported number of channels (%u) in image for texture creation. Only 3 (RGB) and 4 (RGBA) are supported.",
          image.getChannels()
        )
      );

    m_width = image.getWidth();
    m_height = image.getHeight();
    m_channels = image.getChannels();
    m_format = (m_channels == 4) ? GL_RGBA : GL_RGB;
    m_internalFormat = (m_channels == 4) ? GL_RGBA8 : GL_RGB8;

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      m_internalFormat,
      static_cast<Int32>(m_width),
      static_cast<Int32>(m_height),
      0,
      m_format,
      m_type,
      image.getBuffer().data()
    );

    glBindTexture(GL_TEXTURE_2D, 0);
    m_created = true;
  }

  void OpenGlTexture::initialize(UInt32 width, UInt32 height, UInt8 channels)
  {
    if (m_created)
      throw RuntimeErrorException("Texture has already been created, cannot re-initialize.");

    if (width == 0 || height == 0)
      throw InvalidArgumentException(
        String::Format(
          "Invalid image dimensions (%u x %u) for texture creation. Dimensions must be greater than zero.",
          width,
          height
        )
      );

    if (channels != 3 && channels != 4)
      throw InvalidArgumentException(
        String::Format(
          "Unsupported number of channels (%u) in image for texture creation. Only 3 (RGB) and 4 (RGBA) are supported.",
          channels
        )
      );

    m_width = width;
    m_height = height;
    m_channels = channels;
    m_format = (m_channels == 4) ? GL_RGBA : GL_RGB;
    m_internalFormat = (m_channels == 4) ? GL_RGBA8 : GL_RGB8;

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      m_internalFormat,
      static_cast<Int32>(m_width),
      static_cast<Int32>(m_height),
      0,
      m_format,
      m_type,
      nullptr
    );

    glBindTexture(GL_TEXTURE_2D, 0);
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

    m_width = width;
    m_height = height;

    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      m_internalFormat,
      static_cast<Int32>(m_width),
      static_cast<Int32>(m_height),
      0,
      m_format,
      m_type,
      nullptr
    );
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void OpenGlTexture::bind(UInt32 slot) const
  {
    if (!m_created)
      return;

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
  }

  void OpenGlTexture::unbind(UInt32 slot) const
  {
    if (!m_created)
      return;

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  bool OpenGlTexture::isValid() const
  {
    return m_created;
  }

  void OpenGlTexture::destroy()
  {
    if (m_textureId)
    {
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
    GLenum type
  )
  {
    if (m_created)
      throw RuntimeErrorException("Texture has already been created, cannot re-initialize.");

    if (width == 0 || height == 0)
      throw InvalidArgumentException(
        String::Format(
          "Invalid image dimensions (%u x %u) for texture creation. Dimensions must be greater than zero.",
          width,
          height
        )
      );

    m_width = width;
    m_height = height;
    m_channels = (format == GL_RGBA) ? 4 : 3;
    m_format = format;
    m_internalFormat = internalFormat;
    m_type = type;

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      m_internalFormat,
      static_cast<Int32>(m_width),
      static_cast<Int32>(m_height),
      0,
      m_format,
      m_type,
      nullptr
    );

    glBindTexture(GL_TEXTURE_2D, 0);
    m_created = true;
  }

  void OpenGlTexture::assertIsCreated() const
  {
    if (!m_created)
      throw RuntimeErrorException("Texture has not been created yet.");
  }
}
