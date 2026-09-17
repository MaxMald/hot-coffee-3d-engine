#include "hc/graphics/cubeMap/hcOpenGlCubeMap.h"

#include <GL/glew.h>
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  OpenGlCubeMap::OpenGlCubeMap() :
    m_id(0),
    m_valid(false),
    m_descriptor(nullptr)
  {}

  OpenGlCubeMap::~OpenGlCubeMap()
  {
    destroy();
  }

  void OpenGlCubeMap::initialize(
    SharedPtr<CubeMapDescriptor> cubeMapDescriptor,
    IAssetManager& assetManager
  )
  {
    if (m_valid)
      throw RuntimeErrorException("Cube map is already initialized");

    if (!cubeMapDescriptor)
      throw InvalidArgumentException("Cube map descriptor is null");

    UInt32 faceSize = cubeMapDescriptor->faceSize;
    if (faceSize == 0)
      throw RuntimeErrorException("Cube map face size must be greater than zero");

    IImageAssetManager& imageMng = assetManager.getImageAssetManager();

    Path rightImagePath = cubeMapDescriptor->rightImagePath;
    if (rightImagePath.isRelative())
      rightImagePath = rightImagePath.toAbsolute(cubeMapDescriptor->path.parentPath());

    Path leftImagePath = cubeMapDescriptor->leftImagePath;
    if (leftImagePath.isRelative())
      leftImagePath = leftImagePath.toAbsolute(cubeMapDescriptor->path.parentPath());

    Path topImagePath = cubeMapDescriptor->topImagePath;
    if (topImagePath.isRelative())
      topImagePath = topImagePath.toAbsolute(cubeMapDescriptor->path.parentPath());

    Path bottomImagePath = cubeMapDescriptor->bottomImagePath;
    if (bottomImagePath.isRelative())
      bottomImagePath = bottomImagePath.toAbsolute(cubeMapDescriptor->path.parentPath());

    Path backImagePath = cubeMapDescriptor->backImagePath;
    if (backImagePath.isRelative())
      backImagePath = backImagePath.toAbsolute(cubeMapDescriptor->path.parentPath());

    Path frontImagePath = cubeMapDescriptor->frontImagePath;
    if (frontImagePath.isRelative())
      frontImagePath = frontImagePath.toAbsolute(cubeMapDescriptor->path.parentPath());

    SharedPtr<Image> rightImage = imageMng.load(rightImagePath);
    SharedPtr<Image> leftImage = imageMng.load(leftImagePath);
    SharedPtr<Image> topImage = imageMng.load(topImagePath);
    SharedPtr<Image> bottomImage = imageMng.load(bottomImagePath);
    SharedPtr<Image> backImage = imageMng.load(backImagePath);
    SharedPtr<Image> frontImage = imageMng.load(frontImagePath);

    if (rightImage == nullptr)
      throw RuntimeErrorException("Failed to load right image for cube map");
    if (leftImage == nullptr)
      throw RuntimeErrorException("Failed to load left image for cube map");
    if (topImage == nullptr)
      throw RuntimeErrorException("Failed to load top image for cube map");
    if (bottomImage == nullptr)
      throw RuntimeErrorException("Failed to load bottom image for cube map");
    if (backImage == nullptr)
      throw RuntimeErrorException("Failed to load back image for cube map");
    if (frontImage == nullptr)
      throw RuntimeErrorException("Failed to load front image for cube map");

    assertImageSize(*rightImage, faceSize, faceSize);
    assertImageSize(*leftImage, faceSize, faceSize);
    assertImageSize(*topImage, faceSize, faceSize);
    assertImageSize(*bottomImage, faceSize, faceSize);
    assertImageSize(*backImage, faceSize, faceSize);
    assertImageSize(*frontImage, faceSize, faceSize);

    GLint currentCubeMapTexture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &currentCubeMapTexture);

    try
    {
      glGenTextures(1, &m_id);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
      glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,
        openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(rightImage->getFormat(), rightImage->getColorSpace()),
        faceSize, faceSize, 0,
        openGlGraphicsUtilities::GetOpenGlFormatFromTextureFormatType(rightImage->getFormat()),
        GL_UNSIGNED_BYTE, rightImage->getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,
        openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(leftImage->getFormat(), leftImage->getColorSpace()),
        faceSize, faceSize, 0,
        openGlGraphicsUtilities::GetOpenGlFormatFromTextureFormatType(leftImage->getFormat()),
        GL_UNSIGNED_BYTE, leftImage->getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,
        openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(topImage->getFormat(), topImage->getColorSpace()),
        faceSize, faceSize, 0,
        openGlGraphicsUtilities::GetOpenGlFormatFromTextureFormatType(topImage->getFormat()),
        GL_UNSIGNED_BYTE, topImage->getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,
        openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(bottomImage->getFormat(), bottomImage->getColorSpace()),
        faceSize, faceSize, 0,
        openGlGraphicsUtilities::GetOpenGlFormatFromTextureFormatType(bottomImage->getFormat()),
        GL_UNSIGNED_BYTE, bottomImage->getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,
        openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(frontImage->getFormat(), frontImage->getColorSpace()),
        faceSize, faceSize, 0,
        openGlGraphicsUtilities::GetOpenGlFormatFromTextureFormatType(frontImage->getFormat()),
        GL_UNSIGNED_BYTE, frontImage->getBuffer().data()
      );
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,
        openGlGraphicsUtilities::GetOpenGLInternalFormatFromTextureFormatAndColorSpaceType(backImage->getFormat(), backImage->getColorSpace()),
        faceSize, faceSize, 0,
        openGlGraphicsUtilities::GetOpenGlFormatFromTextureFormatType(backImage->getFormat()),
        GL_UNSIGNED_BYTE, backImage->getBuffer().data()
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

    m_descriptor = cubeMapDescriptor;
    m_valid = true;
  }

  bool OpenGlCubeMap::isValid() const
  {
    return m_valid;
  }

  UInt32 OpenGlCubeMap::getFaceWidth() const
  {
    assertIsValid();
    return m_descriptor->faceSize;
  }

  UInt32 OpenGlCubeMap::getFaceHeight() const
  {
    assertIsValid();
    return m_descriptor->faceSize;
  }

  SharedPtr<CubeMapDescriptor> OpenGlCubeMap::getCubeMapDescriptor() const
  {
    assertIsValid();
    return m_descriptor;
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

    m_descriptor.reset();
    m_valid = false;
  }

  UInt32 OpenGlCubeMap::getId() const
  {
    return m_id;
  }

  void OpenGlCubeMap::assertImageSize(
    const Image& image,
    const UInt32 expectedWidth,
    const UInt32 expectedHeight
  )
  {
    if (image.getHeight() != expectedHeight || image.getWidth() != expectedWidth)
      throw RuntimeErrorException("Cube map image has invalid dimensions");
  }

  void OpenGlCubeMap::assertIsValid() const
  {
    if (!m_valid)
      throw RuntimeErrorException("Cube map is not valid");
  }
}
