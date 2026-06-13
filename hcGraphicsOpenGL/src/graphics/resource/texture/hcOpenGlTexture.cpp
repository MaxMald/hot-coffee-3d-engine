#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  OpenGlTexture::OpenGlTexture() :
    m_id(Id::Create()),
    m_textureId(0),
    m_width(0),
    m_height(0),
    m_colorFormat(colorFormatType::RGBA8),
    m_colorSpace(colorSpaceType::Linear),
    m_created(false)
  {}

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
    initialize(
      image.getWidth(),
      image.getHeight(),
      image.getFormat(),
      image.getColorSpace(),
      image.getBuffer().data()
    );
  }

  void OpenGlTexture::initialize(
    UInt32 width,
    UInt32 height,
    colorFormatType::Type colorFormat
  )
  {
    initialize(
      width,
      height,
      colorFormat,
      colorSpaceType::Linear,
      nullptr
    );
  }

  void OpenGlTexture::initialize(
    UInt32 width,
    UInt32 height,
    colorFormatType::Type colorFormat,
    colorSpaceType::Type colorSpace
  )
  {
    initialize(
      width,
      height,
      colorFormat,
      colorSpace,
      nullptr
    );
  }

  void OpenGlTexture::initialize(
    UInt32 width,
    UInt32 height,
    colorFormatType::Type colorFormat,
    colorSpaceType::Type colorSpace,
    const Color& initColor
  )
  {
    if (m_created)
      throw RuntimeErrorException("Texture has already been created, cannot re-initialize.");

    assertDimensionsAreGreaterThanZero(width, height);

    switch (colorFormat)
    {
    case colorFormatType::RGB8:
    {
      BufferByte initData(0);
      TextureBufferFactory::CreateRGB8(width, height, initColor, initData);
      initialize(
        width,
        height,
        colorFormat,
        colorSpace,
        initData.data()
      );
    }
    break;
    case colorFormatType::RGBA8:
    {
      BufferByte initData(0);
      TextureBufferFactory::CreateRGBA8(width, height, initColor, initData);
      initialize(
        width,
        height,
        colorFormat,
        colorSpace,
        initData.data()
      );
    }
    break;
    case colorFormatType::RGB16F:
    {
      BufferFloat initData(0);
      TextureBufferFactory::CreateRGB16F(width, height, initColor, initData);
      initialize(
        width,
        height,
        colorFormat,
        colorSpace,
        initData.data()
      );
    }
    break;
    case colorFormatType::RGBA16F:
    {
      BufferFloat initData(0);
      TextureBufferFactory::CreateRGBA16F(width, height, initColor, initData);
      initialize(
        width,
        height,
        colorFormat,
        colorSpace,
        initData.data()
      );
    }
    break;
    default:
      throw InvalidArgumentException("Unsupported color format type for texture initialization.");
    }
  }

  UInt32 OpenGlTexture::getWidth() const
  {
    return m_width;
  }

  UInt32 OpenGlTexture::getHeight() const
  {
    return m_height;
  }

  colorFormatType::Type OpenGlTexture::getColorFormat() const
  {
    return m_colorFormat;
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
      throw InvalidArgumentException("Texture dimensions must be greater than zero");

    GLint currentTextureId = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId);

    try
    {
      glBindTexture(GL_TEXTURE_2D, m_textureId);
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGLInternalFormatFromColorFormatAndColorSpaceType(m_colorFormat, m_colorSpace)),
        static_cast<Int32>(width), static_cast<Int32>(height),
        0,
        static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGlFormatFromColorFormatType(m_colorFormat)),
        static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGLDataTypeFromColorFormatType(m_colorFormat)),
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
    m_colorFormat = colorFormatType::RGBA8;
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
    UInt32 width,
    UInt32 height,
    colorFormatType::Type colorFormat,
    colorSpaceType::Type colorSpace,
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
        static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGLInternalFormatFromColorFormatAndColorSpaceType(colorFormat, colorSpace)),
        static_cast<Int32>(width), static_cast<Int32>(height),
        0,
        static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGlFormatFromColorFormatType(colorFormat)),
        static_cast<GLenum>(openGlGraphicsUtilities::GetOpenGLDataTypeFromColorFormatType(colorFormat)),
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
    m_colorSpace = colorSpace;
    m_colorFormat = colorFormat;
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
