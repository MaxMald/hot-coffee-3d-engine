#include "hc/graphics/cubeMap/hcOpenGlCubeMap.h"

#include <GL/glew.h>
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  OpenGlCubeMap::OpenGlCubeMap() :
    m_id(0),
    m_valid(false),
    m_faceHeight(0),
    m_faceWidth(0),
    m_format(colorFormatType::Type::RGBA8)
  {}

  OpenGlCubeMap::~OpenGlCubeMap()
  {
    destroy();
  }

  void OpenGlCubeMap::initialize(
    const UInt32 width,
    const UInt32 height,
    const colorFormatType::Type format,
    const Image & right,
    const Image & left,
    const Image & top,
    const Image & bottom,
    const Image & back,
    const Image & front
  )
  {
    if (m_valid)
      throw RuntimeErrorException("Cube map is already initialized");

    if (width == 0 || height == 0)
      throw RuntimeErrorException("Cube map dimensions must be greater than zero");

    assertImageSize(right, width, height);
    assertImageSize(left, width, height);
    assertImageSize(top, width, height);
    assertImageSize(bottom, width, height);
    assertImageSize(back, width, height);
    assertImageSize(front, width, height);

    GLenum glFormat = openGlGraphicsUtilities::GetOpenGlFormatFromColorFormatType(format);
    GLenum internalFormat = (glFormat == GL_RGB) ? GL_RGB8 : GL_RGBA8;
    GLint currentCubeMapTexture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &currentCubeMapTexture);

    try
    {
      glGenTextures(1, &m_id);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
      glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat,
        width, height, 0, glFormat,
        GL_UNSIGNED_BYTE, right.getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat,
        width, height, 0, glFormat,
        GL_UNSIGNED_BYTE, left.getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat,
        width, height, 0, glFormat,
        GL_UNSIGNED_BYTE, top.getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat,
        width, height, 0, glFormat,
        GL_UNSIGNED_BYTE, bottom.getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat,
        width, height, 0, glFormat,
        GL_UNSIGNED_BYTE, back.getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat,
        width, height, 0, glFormat,
        GL_UNSIGNED_BYTE, front.getBuffer().data()
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
    m_faceWidth = width;
    m_faceHeight = height;
    m_format = format;
    m_valid = true;
  }

  bool OpenGlCubeMap::isValid() const
  {
    return m_valid;
  }

  UInt32 OpenGlCubeMap::getFaceWidth() const
  {
    return m_faceWidth;
  }

  UInt32 OpenGlCubeMap::getFaceHeight() const
  {
    return m_faceHeight;
  }

  colorFormatType::Type OpenGlCubeMap::getFormat() const
  {
    return colorFormatType::Type();
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

    m_format = colorFormatType::Type::RGBA8;
    m_faceHeight = 0;
    m_faceWidth = 0;
    m_valid = false;
  }

  UInt32 OpenGlCubeMap::getId() const
  {
    return m_id;
  }

  void OpenGlCubeMap::assertImageSize(
    const Image & image,
    const UInt32 width,
    const UInt32 height
  )
  {
    if (image.getHeight() != height || image.getWidth() != width)
      throw RuntimeErrorException("Cube map image has invalid dimensions");
  }

  void OpenGlCubeMap::assertIsValid() const
  {
    if (!m_valid)
      throw RuntimeErrorException("Cube map is not valid");
  }
}
