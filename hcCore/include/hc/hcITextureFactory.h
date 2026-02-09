#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Image;
  class ITexture;

  /**
   * @brief Interface for texture factory classes.
   *
   * Provides an abstract interface for creating texture instances from images.
   * Implementations should provide concrete texture creation logic.
   */
  class HC_CORE_EXPORT ITextureFactory
  {
  public:
    virtual ~ITextureFactory() = default;

    /**
     * @brief Creates a texture from the given image.
     *
     * @param image Shared pointer to the image used for texture creation.
     *
     * @return Shared pointer to the created texture instance. nullptr if
     * creation fails.
     */
    virtual SharedPtr<ITexture> createTexture(SharedPtr<Image> image) = 0;

   protected:
     ITextureFactory() = default;
  };

}
