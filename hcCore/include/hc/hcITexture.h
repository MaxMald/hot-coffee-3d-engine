#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Interface for texture resources in the graphics module.
   */
  class ITexture
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
     * @brief Destroys the texture and releases resources.
     */
    virtual void destroy() = 0;

    /**
     * @brief Gets the native graphics API handle for the texture.
     * 
     * @return Pointer to the native texture handle.
     */
    virtual void* getNativeHandle() const = 0;
  };
}
