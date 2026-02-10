#pragma once

#include "hc/hcCorePrerequisites.h"

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
