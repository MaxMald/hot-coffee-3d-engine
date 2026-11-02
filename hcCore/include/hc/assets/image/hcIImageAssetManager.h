#pragma once

#include "hc/hcCoreCommons.h"
#include "hc/assets/hcATypedAssetManager.h"
#include "hc/assets/image/hcImage.h"

namespace hc
{
  namespace defaultImageType
  {
    /**
     * @brief Enumeration of default image types.
     */
    enum Type : UInt8
    {
      White,
      NormalTangent
    };
  }

  /**
   * @brief Interface for managing image assets.
   */
  class HC_CORE_EXPORT IImageAssetManager : public ATypedAssetManager<Image>
  {
  public:
    virtual ~IImageAssetManager() = default;

    /**
     * @brief Checks if the specified image file is supported by the asset manager.
     *
     * @param path The file path to the image.
     *
     * @return true if the image format is supported, false otherwise.
     */
    virtual bool isSupportedImage(const Path& path) const = 0;

    /**
     * @brief Retrieves a default image based on the specified type, format, and color
     * space. Default images have a size of 1x1 pixel and are used as placeholders or for
     * specific rendering purposes.
     *
     * @param type The type of default image to retrieve.
     * @param format The texture format of the default image.
     * @param colorSpace The color space of the default image.
     *
     * @return A shared pointer to the requested default image.
     */
    SharedPtr<Image> getDefaultImage(
      defaultImageType::Type type,
      textureFormatType::Type format,
      colorSpaceType::Type colorSpace
    );

  protected:
    UnorderedMap<UInt32, SharedPtr<Image>> m_defaultImages; ///< Default images mapped by a unique key derived from their type, format, and color space.

    IImageAssetManager() = default;
  };
}
