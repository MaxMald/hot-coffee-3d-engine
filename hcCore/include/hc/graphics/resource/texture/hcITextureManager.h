/**
 * @todo Fix texture caching for images loaded with different color spaces.
 *
 * createTextureFromFile() currently mutates the cached Image's color space
 * before forwarding to createTextureFromImage(). Textures are cached using
 * only the Image identifier, so requesting the same image with different
 * color spaces may return an existing texture created with a different
 * color space than requested.
 *
 * Additionally, mutating the shared Image instance can affect other systems
 * that reference the same cached image.
 *
 * Possible solutions:
 * - Include color space in the texture cache key.
 * - Avoid mutating cached Image instances.
 * - Treat image color space as immutable after loading.
 */

#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/utilities/hcColorSpaceType.h"

namespace hc
{
  class Image;
  class ITexture;

  /**
   * @brief Interface for managing textures in the engine.
   *
   * The ITextureManager is responsible for creating, managing, and clearing
   * textures. It provides methods to create textures from images, image keys, or
   * files, and to clear all managed textures.
   */
  class HC_CORE_EXPORT ITextureManager : public NonCopyable
  {
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~ITextureManager() = default;

    /**
    * @brief Creates an uninitialized-empty texture.
    *
    * @return Shared pointer to the created texture.
    */
    virtual SharedPtr<ITexture> createTexture() = 0;

    /**
     * @brief Creates a texture from a given image.
     *
     * @param image Shared pointer to the source image.
     * 
     * @return Shared pointer to the created texture.
     */
    virtual SharedPtr<ITexture> createTextureFromImage(SharedPtr<Image> image) = 0;

    /**
     * @brief Creates a texture from an image file.
     *
     * @param filePath Path to the image file.
     *
     * @return Shared pointer to the created texture.
     */
    virtual SharedPtr<ITexture> createTextureFromFile(const Path& filePath) = 0;

    /**
     * @brief Creates a texture from an image file.
     *
     * @param filePath Path to the image file.
     * @param colorSpace Color space of the texture.
     * 
     * @return Shared pointer to the created texture.
     */
    virtual SharedPtr<ITexture> createTextureFromFile(
      const Path& filePath,
      colorSpaceType::Type colorSpace
    ) = 0;

    /**
     * @brief Gets a list of all managed textures.
     *
     * @return Vector of shared pointers to the managed textures.
     */
    virtual const Vector<SharedPtr<ITexture>>& getTextures() = 0;

    /**
     * @brief Clears all managed textures.
     */
    virtual void clear() = 0;
  };
}
