#pragma once

#include "hc/utilities/hcTextureFormatType.h"
#include "hc/utilities/hcColorSpaceType.h"
#include "hc/graphics/resource/hcIGraphicResource.h"

namespace hc
{
  class Image;

  /**
   * @brief Interface for texture resources in the graphics module.
   */
  class ITexture : public IGraphicResource
  {
  public:
    virtual ~ITexture() = default;

    /**
     * @brief Initializes the texture with the given image data. The texture will be
     * created based on the dimensions and pixel data of the provided image.
     *
     * @param image The image asset containing the pixel data and dimensions for the
     * texture.
     */
    virtual void initialize(const Image& image) = 0;

    /**
     * @brief Initializes the texture with the given dimensions. The texture will be
     * created with empty pixel data. Color space will default to Linear.
     *
     * @param width The width of the texture in pixels.
     * @param height The height of the texture in pixels.
     * @param format The texture format of the texture.
     */
    virtual void initialize(
      UInt32 width,
      UInt32 height,
      textureFormatType::Type format
    ) = 0;

    /**
     * @brief Initializes the texture with the given dimensions and color space. The
     * texture will be created with empty pixel data.
     *
     * @param width The width of the texture in pixels.
     * @param height The height of the texture in pixels.
     * @param format The texture format of the texture.
     * @param colorSpace The color space of the texture (e.g., sRGB, Linear).
     */
    virtual void initialize(
      UInt32 width,
      UInt32 height,
      textureFormatType::Type format,
      colorSpaceType::Type colorSpace
    ) = 0;

    /**
     * @brief Initializes the texture with the given dimensions and fills it with the
     * specified color. The texture will be created with pixel data initialized to the
     * provided color.
     *
     * @param width The width of the texture in pixels.
     * @param height The height of the texture in pixels.
     * @param format The color format of the texture.
     * @param colorSpace The color space of the texture (e.g., sRGB, Linear).
     * @param initColor The color to initialize the texture's pixel data with.
     */
    virtual void initialize(
      UInt32 width,
      UInt32 height,
      textureFormatType::Type format,
      colorSpaceType::Type colorSpace,
      const Color& initColor
    ) = 0;

    /**
     * @brief Gets the width of the texture in pixels.
     *
     * @return Texture width as an unsigned 32-bit integer.
     */
    virtual UInt32 getWidth() const = 0;

    /**
     * @brief Gets the height of the texture in pixels.
     *
     * @return Texture height as an unsigned 32-bit integer.
     */
    virtual UInt32 getHeight() const = 0;

    /**
     * @brief Gets the texture format of the texture (e.g., RGBA8, RGB8).
     *
     * @return The texture format type of the texture.
     */
    virtual textureFormatType::Type getTextureFormat() const = 0;

    /**
     * @brief Gets the color space of the texture (e.g., sRGB, Linear).
     *
     * @return The color space type of the texture.
     */
    virtual colorSpaceType::Type getColorSpace() const = 0;

    /**
     * @brief Resizes the texture to the specified dimensions. Resizing a texture created
     * from an image is not allowed, create a new texture instead.
     *
     * @param width The new width of the texture in pixels.
     * @param height The new height of the texture in pixels.
     */
    virtual void resize(UInt32 width, UInt32 height) = 0;

    /**
     * @brief Binds the texture to the specified slot for rendering.
     *
     * @param slot The texture slot to bind to (default is 0).
     */
    virtual void bind(UInt32 slot = 0) const = 0;

    /**
     * @brief Unbinds the texture from the specified slot.
     *
     * @param slot The texture slot to unbind from (default is 0).
     */
    virtual void unbind(UInt32 slot = 0) const = 0;

    /**
     * @brief Checks if the texture resource is valid and loaded.
     *
     * @return True if the texture is valid, false otherwise.
     */
    virtual bool isValid() const = 0;

    /**
     * @brief Gets the native graphics API handle for the texture.
     *
     * @return Pointer to the native texture handle.
     */
    virtual void* getNativeHandle() const = 0;
  };
}
