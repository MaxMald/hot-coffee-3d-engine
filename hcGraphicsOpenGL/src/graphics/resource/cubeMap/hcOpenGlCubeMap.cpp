#include "hc/graphics/resource/cubeMap/hcOpenGlCubeMap.h"

#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  OpenGlCubeMap::OpenGlCubeMap() :
    m_sourcePath(),
    m_id(0),
    m_faceSize(0),
    m_textureFormat(textureFormatType::RGBA8),
    m_colorSpace(colorSpaceType::Linear),
    m_valid(false)
  {}

  OpenGlCubeMap::~OpenGlCubeMap()
  {
    destroy();
  }

  void OpenGlCubeMap::initialize(
    const Image& px,
    const Image& nx,
    const Image& py,
    const Image& ny,
    const Image& pz,
    const Image& nz,
    const Path& sourcePath
  )
  {
    if (m_valid)
      throw RuntimeErrorException("Cube map is already initialized");

    UInt32 faceSize = px.getWidth();
    textureFormatType::Type format = px.getFormat();
    colorSpaceType::Type colorSpace = px.getColorSpace();
    assertImage(px, faceSize, format, colorSpace);
    assertImage(nx, faceSize, format, colorSpace);
    assertImage(py, faceSize, format, colorSpace);
    assertImage(ny, faceSize, format, colorSpace);
    assertImage(pz, faceSize, format, colorSpace);
    assertImage(nz, faceSize, format, colorSpace);

    GLint currentCubeMapTexture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &currentCubeMapTexture);

    GLint glInternalFormat = openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(format, colorSpace);
    GLenum glFormat = openGlGraphicsUtilities::GetOpenGlFormatFromTextureFormatType(format);
    GLenum glType = openGlGraphicsUtilities::GetOpenGLDataTypeFromTextureFormatType(format);

    try
    {
      glGenTextures(1, &m_id);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
      glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,
        glInternalFormat,
        faceSize, faceSize, 0,
        glFormat,
        glType, px.getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,
        glInternalFormat,
        faceSize, faceSize, 0,
        glFormat,
        glType, nx.getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,
        glInternalFormat,
        faceSize, faceSize, 0,
        glFormat,
        glType, py.getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,
        glInternalFormat,
        faceSize, faceSize, 0,
        glFormat,
        glType, ny.getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,
        glInternalFormat,
        faceSize, faceSize, 0,
        glFormat,
        glType, pz.getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,
        glInternalFormat,
        faceSize, faceSize, 0,
        glFormat,
        glType, nz.getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
    }
    catch (...)
    {
      glBindTexture(GL_TEXTURE_CUBE_MAP, currentCubeMapTexture);
      destroy();
      throw;
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, currentCubeMapTexture);

    m_faceSize = faceSize;
    m_textureFormat = format;
    m_colorSpace = colorSpace;
    m_sourcePath = sourcePath;
    m_valid = true;
  }

  void OpenGlCubeMap::initialize(
    UInt32 faceSize,
    textureFormatType::Type format,
    colorSpaceType::Type colorSpace
  )
  {
    if (m_valid)
      throw RuntimeErrorException("Cube map is already initialized");

    if (faceSize == 0)
      throw InvalidArgumentException("Face size must be greater than zero.");

    GLint currentCubeMapTexture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &currentCubeMapTexture);

    GLint glInternalFormat = openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(format, colorSpace);
    GLenum glFormat = openGlGraphicsUtilities::GetOpenGlFormatFromTextureFormatType(format);
    GLenum glType = openGlGraphicsUtilities::GetOpenGLDataTypeFromTextureFormatType(format);

    try
    {
      glGenTextures(1, &m_id);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
      glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

      for (int i = 0; i < 6; ++i)
      {
        glTexImage2D(
          GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
          glInternalFormat,
          faceSize, faceSize, 0,
          glFormat,
          glType, nullptr
        );
        openGlGraphicsUtilities::AssertOpenGlHasNoError();
      }

      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
    }
    catch (...)
    {
      glBindTexture(GL_TEXTURE_CUBE_MAP, currentCubeMapTexture);
      destroy();
      throw;
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, currentCubeMapTexture);

    m_faceSize = faceSize;
    m_textureFormat = format;
    m_colorSpace = colorSpace;
    m_sourcePath.clear();
    m_valid = true;
  }

  bool OpenGlCubeMap::isValid() const
  {
    return m_valid;
  }

  void OpenGlCubeMap::destroy()
  {
    if (m_id != 0)
    {
      GLint currentTexture = 0;
      glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &currentTexture);
      if (currentTexture == static_cast<GLint>(m_id))
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

      glDeleteTextures(1, &m_id);
      m_id = 0;
    }

    m_faceSize = 0;
    m_sourcePath.clear();
    m_valid = false;
  }

  const UUID& OpenGlCubeMap::getUUID() const
  {
    return m_uuid;
  }

  UInt32 OpenGlCubeMap::getFaceSize() const
  {
    return m_faceSize;
  }

  textureFormatType::Type OpenGlCubeMap::getTextureFormat() const
  {
    return m_textureFormat;
  }

  colorSpaceType::Type OpenGlCubeMap::getColorSpace() const
  {
    return m_colorSpace;
  }

  const Path& OpenGlCubeMap::getSourcePath() const
  {
    return m_sourcePath;
  }
}
