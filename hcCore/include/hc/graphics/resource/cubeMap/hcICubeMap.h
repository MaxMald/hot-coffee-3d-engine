#pragma once

#include "hc/hcCoreCommons.h"
#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"
#include "hc/graphics/resource/hcIGraphicResource.h"

namespace hc
{
  class Image;

  /**
   * @brief Interface for cube map resources in the graphics module.
   */
  class HC_CORE_EXPORT ICubeMap : public IGraphicResource
  {
  public:
    virtual ~ICubeMap();

    /**
     * @brief Initializes the cube map with the given images for each face.
     *
     * @param rightFace The image for the right face of the cube map.
     * @param leftFace The image for the left face of the cube map.
     * @param topFace The image for the top face of the cube map.
     * @param bottomFace The image for the bottom face of the cube map.
     * @param backFace The image for the back face of the cube map.
     * @param frontFace The image for the front face of the cube map.
     * @param sourcePath Optional source path of the cube map if it was loaded from a file
     * descriptor.
     *
     * @throw InvalidArgumentException if any of the provided images are invalid or if the
     * face sizes do not match.
     * @throw RuntimeException if the cube map fails to initialize due to graphics API
     * errors.
     */
    virtual void initialize(
      const Image& rightFace,
      const Image& leftFace,
      const Image& topFace,
      const Image& bottomFace,
      const Image& backFace,
      const Image& frontFace,
      const Path& sourcePath = Path()
    ) = 0;

    /**
     * @brief Initializes the cube map with the specified face size, texture format, and
     * color space. Data for the cube map faces will be uninitialized and must be filled
     * later.
     *
     * @param faceSize The size of each face of the cube map (width and height).
     * @param format The texture format of the cube map (e.g., RGB8, RGBA8, Depth24).
     * @param colorSpace The color space of the cube map (e.g., sRGB, Linear).
     *
     * @throw InvalidArgumentException if the face size is zero or if the format/color
     * space is invalid.
     * @throw RuntimeException if the cube map fails to initialize due to graphics API
     * errors.
     */
    virtual void initialize(
      UInt32 faceSize,
      textureFormatType::Type format,
      colorSpaceType::Type colorSpace
    ) = 0;

    /**
     * @brief Gets the size of the faces of the cube map in pixels.
     * @return The size of each face of the cube map (width and height).
     */
    virtual UInt32 getFaceSize() const = 0;

    /**
     * @brief Gets the texture format of the cube map.
     * @return The texture format of the cube map (e.g., RGB8, RGBA8, Depth24).
     */
    virtual textureFormatType::Type getTextureFormat() const = 0;

    /**
     * @brief Gets the color space of the cube map.
     * @return The color space of the cube map (e.g., sRGB, Linear).
     */
    virtual colorSpaceType::Type getColorSpace() const = 0;

    /**
     * @brief Gets the source path of the CubeMap if it was loaded from a file descriptor.
     * @return The source path of the CubeMap or an empty path if it was not loaded from a
     * file descriptor.
     */
    virtual const Path& getSourcePath() const = 0;

  protected:
    ICubeMap();
  };
}
