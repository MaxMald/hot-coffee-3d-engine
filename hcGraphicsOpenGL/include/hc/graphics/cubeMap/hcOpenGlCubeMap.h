#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  /**
   * @brief Represents an OpenGL cube map resource.
   */
  class OpenGlCubeMap : public ICubeMap
  {
  public:
    OpenGlCubeMap();
    ~OpenGlCubeMap() override;

    /**
     * @copydoc ICubeMap::initialize
     */
    void initialize(
      const Image& rightFace,
      const Image& leftFace,
      const Image& topFace,
      const Image& bottomFace,
      const Image& backFace,
      const Image& frontFace,
      const Path& sourcePath = Path()
    ) override;

    /**
     * @copydoc IGraphicResource::isValid
     */
    bool isValid() const override;

    /**
     * @copydoc IGraphicResource::destroy
     */
    void destroy() override;

    /**
     * @copydoc IGraphicResource::getUUID
     */
    const UUID& getUUID() const;

    /**
     * @copydoc ICubeMap::getSourcePath
     */
    UInt32 getFaceSize() const override;

    /**
     * @copydoc ICubeMap::getTextureFormat
     */
    textureFormatType::Type getTextureFormat() const override;

    /**
     * @copydoc ICubeMap::getColorSpace
     */
    colorSpaceType::Type  getColorSpace() const override;

    /**
     * @copydoc ICubeMap::getSourcePath
     */
    const Path& getSourcePath() const override;

    /**
     * @brief Returns the OpenGL texture ID for the cube map.
     * @returns The OpenGL texture ID.
     */
    inline UInt32 getId() const
    {
      return m_id;
    }

  private:
    Path m_sourcePath;
    UInt32 m_id;
    UInt32 m_faceSize;
    textureFormatType::Type m_textureFormat;
    colorSpaceType::Type m_colorSpace;
    bool m_valid;

    /**
     * @brief Asserts that the provided image has the expected properties.
     *
     * @param image The image to check.
     * @param expectedFaceSize The expected size of the cube map face.
     * @param expectedFormat The expected texture format of the image.
     * @param expectedColorSpace The expected color space of the image.
     *
     * @throws InvalidArgumentException if the image size does not match the expected face
     * size.
     */
    static inline void assertImage(
      const Image& image,
      UInt32 expectedFaceSize,
      textureFormatType::Type expectedFormat,
      colorSpaceType::Type expectedColorSpace
    )
    {
      if (image.getWidth() != expectedFaceSize || image.getHeight() != expectedFaceSize)
      {
        throw InvalidArgumentException(
          "Image size does not match the expected cube map face size."
        );
      }
      if (image.getFormat() != expectedFormat)
      {
        throw InvalidArgumentException(
          "Image format does not match the expected cube map face format."
        );
      }
      if (image.getColorSpace() != expectedColorSpace)
      {
        throw InvalidArgumentException(
          "Image color space does not match the expected cube map face color space."
        );
      }
    }

    /**
     * @brief Asserts that the cube map is valid and properly initialized.
     *
     * @throws RuntimeException if the cube map is not valid.
     */
    inline void assertIsValid() const
    {
      if (!isValid())
        throw RuntimeErrorException("Cube map is not valid.");
    }
  };
}
