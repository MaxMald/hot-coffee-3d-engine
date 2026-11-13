#include "hc/graphics/resource/textureArray/hcOpenGlTextureArray.h"
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc::graphics::resource::textureArray
{
  OpenGlTextureArray::OpenGlTextureArray() :
    m_textureArrayId(0),
    m_width(0),
    m_height(0),
    m_layerCount(0),
    m_textureFormat(textureFormatType::RGBA8),
    m_colorSpace(colorSpaceType::Linear),
    m_isValid(false)
  {
  }

  OpenGlTextureArray::~OpenGlTextureArray()
  {
    destroy();
  }

  void OpenGlTextureArray::initialize(
    const TextureArrayInitializationSettings& initializationSettings
  )
  {
    destroy();

    String errorMessage;
    if (!initializationSettings.isValid(errorMessage))
      throw InvalidArgumentException("OpenGlTextureArray: Invalid initialization settings: " + errorMessage);

    GLint currentTextureArrayId = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &currentTextureArrayId);

    try
    {
      glGenTextures(1, &m_textureArrayId);
      glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureArrayId);

      GLenum internalFormat = openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(
        initializationSettings.format,
        initializationSettings.colorSpace
      );

      glTexStorage3D(
        GL_TEXTURE_2D_ARRAY,
        1,
        internalFormat,
        initializationSettings.width,
        initializationSettings.height,
        initializationSettings.layerCount
      );

      GLint minFilter = openGlGraphicsUtilities::GetOpenGlMinFilter(
        initializationSettings.minFilter,
        initializationSettings.mipmapMode
      );
      GLint magFilter = openGlGraphicsUtilities::GetOpenGlMagFilter(initializationSettings.magFilter);
      GLint wrapMode = openGlGraphicsUtilities::GetOpenGlWrapMode(initializationSettings.wrap);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilter);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilter);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapMode);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapMode);
      if (initializationSettings.mipmapMode != mipmapMode::None)
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }
    catch (const Exception& ex)
    {
      destroy();
      glBindTexture(GL_TEXTURE_2D_ARRAY, currentTextureArrayId);
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, currentTextureArrayId);

    m_width = initializationSettings.width;
    m_height = initializationSettings.height;
    m_layerCount = initializationSettings.layerCount;
    m_textureFormat = initializationSettings.format;
    m_colorSpace = initializationSettings.colorSpace;
    m_isValid = true;
  }

  void OpenGlTextureArray::destroy()
  {
    if (m_textureArrayId != 0)
    {
      GLint currentTextureArrayId = 0;
      glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &currentTextureArrayId);

      if (currentTextureArrayId == static_cast<GLint>(m_textureArrayId))
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

      glDeleteTextures(1, &m_textureArrayId);
      m_textureArrayId = 0;
    }

    m_width = 0;
    m_height = 0;
    m_layerCount = 0;
    m_textureFormat = textureFormatType::RGBA8;
    m_colorSpace = colorSpaceType::Linear;
    m_isValid = false;
  }
}
