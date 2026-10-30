#pragma once

#include "hc/hcCoreCommons.h"
#include "hc/graphics/hcGraphicsCommons.h"
#include "hc/graphics/resource/hcIGraphicResource.h"

namespace hc
{
  class Image;

  /**
   * @brief Structure representing the initialization settings for a texture.
   */
  struct HC_CORE_EXPORT TextureInitializationSettings
  {
    UInt16 width = 1;                                           ///< The width of the texture in pixels.
    UInt16 height = 1;                                          ///< The height of the texture in pixels.
    UInt16 mipmapMaxLevel = 1000;                               ///< The maximum mipmap level for the texture. This is the highest level that will be generated when mipmaps are created.
    textureFormatType::Type format = textureFormatType::RGBA8;  ///< The texture format of the texture (e.g., RGB8, RGBA8, Depth24).
    colorSpaceType::Type colorSpace = colorSpaceType::Linear;   ///< The color space of the texture (e.g., sRGB, Linear).
    filterMode::Type minFilter = filterMode::Linear;            ///< The minification filter mode for the texture. Determines how the texture is sampled when it is scaled down.
    filterMode::Type magFilter = filterMode::Linear;            ///< The magnification filter mode for the texture. Determines how the texture is sampled when it is scaled up.
    mipmapMode::Type mipmapMode = mipmapMode::None;             ///< The mipmap mode for the texture. Determines how mipmaps are sampled.
    wrapMode::Type wrap = wrapMode::Repeat;                     ///< The wrap mode for the texture. Determines how texture coordinates outside the [0, 1] range are handled.

    /**
     * @brief Default constructor for TextureInitializationSettings.
     * Initializes the texture settings with default values.
     */
    TextureInitializationSettings();

    /**
     * @brief Constructor for TextureInitializationSettings that initializes the settings
     * based on the provided image.
     * @param image The image asset from which to derive the texture settings.
     */
    explicit TextureInitializationSettings(const Image& image);
  };

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
     * @param initializationSettings Initialization settings for the texture.
     * @param sourcePath Optional source path of the texture if it was loaded from a file.
     */
    virtual void initialize(
      const Image& image,
      const TextureInitializationSettings& initializationSettings,
      const Path& sourcePath
    ) = 0;

    /**
     * @brief Initializes the texture with the specified width, height, format, and color
     * space. This method is used to create an empty texture that can be filled later.
     *
     * @param initializationSettings Initialization settings for the texture.
     */
    virtual void initialize(
      const TextureInitializationSettings& initializationSettings
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

    /**
     * @brief Gets the source path of the texture if it was loaded from a file.
     *
     * @return Reference to the source path of the texture.
     */
    virtual const Path& getSourcePath() const = 0;
  };
}
