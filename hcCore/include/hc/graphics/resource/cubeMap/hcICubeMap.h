#pragma once

#include "hc/hcCoreCommons.h"
#include "hc/graphics/hcGraphicsCommons.h"
#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"
#include "hc/graphics/resource/hcIGraphicResource.h"

namespace hc
{
  class Image;

  /**
   * @brief Structure representing the initialization settings for a cube map.
   */
  struct CubeMapInitializationSettings
  {
    UInt16 faceSize = 128;                            ///< The size of each face of the cube map in pixels.
    UInt16 mipmapBaseLevel = 0;                       ///< The base mipmap level for the cube map. This is the level that will be used for rendering when no mipmaps are generated.
    UInt16 mipmapMaxLevel = 1000;                     ///< The maximum mipmap level for the cube map. This is the highest level that will be generated when mipmaps are created.
    textureFormatType::Type format;                   ///< The texture format of the cube map (e.g., RGB8, RGBA8, Depth24).
    colorSpaceType::Type colorSpace;                  ///< The color space of the cube map (e.g., sRGB, Linear).
    filterMode::Type minFilter = filterMode::Linear;  ///< The minification filter mode for the cube map. Determines how the texture is sampled when it is scaled down.
    filterMode::Type magFilter = filterMode::Linear;  ///< The magnification filter mode for the cube map. Determines how the texture is sampled when it is scaled up.
    mipmapMode::Type mipmapMode = mipmapMode::None;   ///< The mipmap mode for the cube map. Determines how mipmaps are sampled.
    wrapMode::Type wrap = wrapMode::ClampToEdge;      ///< The wrap mode for the cube map. Determines how texture coordinates outside the [0, 1] range are handled.
  };

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
     * @param px The image for the positive X face of the cube map.
     * @param nx The image for the negative X face of the cube map.
     * @param py The image for the positive Y face of the cube map.
     * @param ny The image for the negative Y face of the cube map.
     * @param pz The image for the positive Z face of the cube map.
     * @param nz The image for the negative Z face of the cube map.
     * @param initializationSettings Initialization settings for the cube map.
     * @param sourcePath Optional source path of the cube map if it was loaded from a file
     * descriptor.
     *
     * @note The provided images must have the same dimensions and format specified in the
     * initialization settings.
     *
     * @throw InvalidArgumentException if any of the provided images are invalid or if the
     * face sizes do not match.
     * @throw RuntimeException if the cube map fails to initialize due to graphics API
     * errors.
     */
    virtual void initialize(
      const Image& px,
      const Image& nx,
      const Image& py,
      const Image& ny,
      const Image& pz,
      const Image& nz,
      const CubeMapInitializationSettings& initializationSettings,
      const Path & sourcePath = Path()
    ) = 0;

    /**
     * @brief Initializes the cube map with the specified settings. Data for the cube map
     * faces will be uninitialized and must be filled later.
     *
     * @param initializationSettings Initialization settings for the cube map.
     *
     * @throw InvalidArgumentException if the face size is zero or if the format/color
     * space is invalid.
     * @throw RuntimeException if the cube map fails to initialize due to graphics API
     * errors.
     */
    virtual void initialize(
      const CubeMapInitializationSettings& initializationSettings
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
     * @brief Binds the cube map to the specified texture slot for rendering.
     * @param slot The texture slot to bind the cube map to.
     */
    virtual void bind(UInt32 slot) const = 0;

    /**
     * @brief Generates mipmaps for the cube map. This should be called after the cube map
     * has been initialized and filled with data.
     * @throw RuntimeException if mipmap generation fails due to graphics API errors.
     */
    virtual void generateMipmaps() = 0;

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
