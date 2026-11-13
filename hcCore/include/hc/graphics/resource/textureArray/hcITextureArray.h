#pragma once

#include "hc/hcCoreCommons.h"
#include "hc/graphics/hcGraphicsCommons.h"
#include "hc/graphics/resource/hcIGraphicResource.h"

namespace hc::graphics::resource::textureArray
{
  /**
   * @brief Structure representing the initialization settings for a texture array.
   */
  struct TextureArrayInitializationSettings
  {
    UInt32 width = 1;                                           ///< The width of the texture array in pixels.
    UInt32 height = 1;                                          ///< The height of the texture array in pixels.
    UInt32 layerCount = 1;                                      ///< The number of layers in the texture array.
    UInt32 mipmapMaxLevel = 1000;                               ///< The maximum mipmap level for the texture array. This is the highest level that will be generated when mipmaps are created.
    textureFormatType::Type format = textureFormatType::RGBA8;  ///< The texture format of the texture array (e.g., RGB8, RGBA8, Depth24).
    colorSpaceType::Type colorSpace = colorSpaceType::Linear;   ///< The color space of the texture array (e.g., sRGB, Linear).
    filterMode::Type minFilter = filterMode::Linear;            ///< The minification filter mode for the texture array. Determines how the texture is sampled when it is scaled down.
    filterMode::Type magFilter = filterMode::Linear;            ///< The magnification filter mode for the texture array. Determines how the texture is sampled when it is scaled up.
    mipmapMode::Type mipmapMode = mipmapMode::None;             ///< The mipmap mode for the texture array. Determines how mipmaps are sampled.
    wrapMode::Type wrap = wrapMode::Repeat;                     ///< The wrap mode for the texture array. Determines how texture coordinates outside the [0, 1] range are handled.

    /**
     * @brief Indicates whether the initialization settings are valid.
     * @param outErrorMessage A string to hold the error message if the settings are
     * invalid.
     * @returns True if the settings are valid, false otherwise.
     */
    inline bool isValid(String& outErrorMessage) const
    {
      if (width == 0)
      {
        outErrorMessage = "Width must be greater than zero.";
        return false;
      }
      if (height == 0)
      {
        outErrorMessage = "Height must be greater than zero.";
        return false;
      }
      if (layerCount == 0)
      {
        outErrorMessage = "Layer count must be greater than zero.";
        return false;
      }
      return true;
    }
  };

  /**
   * @brief Interface for texture array resources in the graphics module.
   */
  class ITextureArray : public IGraphicResource
  {
  public:
    virtual ~ITextureArray() = default;

    /**
     * @brief Initializes the texture array with the given initialization settings.
     * @param initializationSettings Initialization settings for the texture array.
     */
    virtual void initialize(
      const TextureArrayInitializationSettings& initializationSettings
    ) = 0;

    /**
     * @brief Retrieves the width of the texture array in pixels.
     * @returns The width of the texture array.
     */
    virtual UInt32 getWidth() const = 0;

    /**
     * @brief Retrieves the height of the texture array in pixels.
     * @returns The height of the texture array.
     */
    virtual UInt32 getHeight() const = 0;

    /**
     * @brief Retrieves the number of layers in the texture array.
     * @returns The number of layers in the texture array.
     */
    virtual UInt32 getLayerCount() const = 0;

    /**
     * @brief Retrieves the texture format of the texture array (e.g., RGBA8, RGB8).
     * @returns The texture format type of the texture array.
     */
    virtual textureFormatType::Type getTextureFormat() const = 0;

    /**
     * @brief Retrieves the color space of the texture array (e.g., sRGB, Linear).
     * @returns The color space type of the texture array.
     */
    virtual colorSpaceType::Type getColorSpace() const = 0;

  protected:
    ITextureArray() = default;
  };
}
