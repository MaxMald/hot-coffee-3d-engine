#pragma once

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
     * @brief Checks if the texture was created from an image.
     * 
     * @return True if the texture is image-based, false otherwise.
     */
    virtual bool isImageBased() const = 0;

    /**
     * @brief Gets the image associated with the texture.
     * 
     * @return Shared pointer to the image used to create the texture.
     */
    virtual SharedPtr<Image> getImage() = 0;

    /**
     * @brief Gets the native graphics API handle for the texture.
     * 
     * @return Pointer to the native texture handle.
     */
    virtual void* getNativeHandle() const = 0;
  };
}
