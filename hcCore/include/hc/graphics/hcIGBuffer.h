#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class ITexture;

  /**
   * Interface for geometry buffer resources used by deferred rendering.
   */
  class HC_CORE_EXPORT IGBuffer
  {
  public:
    virtual ~IGBuffer() = default;

    /**
     * Creates and initializes attachments with the given dimensions.
     *
     * @param width Buffer width in pixels.
     * @param height Buffer height in pixels.
     * @param textureManager Reference to the texture manager for creating texture
     * attachments.
     */
    virtual void initialize(UInt32 width, UInt32 height) = 0;

    /**
     * Binds the geometry buffer as the active render target.
     */
    virtual void bindForWriting() = 0;

    /**
     * Binds the geometry buffer attachments for sampling.
     */
    virtual void bindForReading() = 0;

    /**
     * Clears all geometry buffer attachments.
     */
    virtual void clear() = 0;

    /**
     * Unbinds the geometry buffer from the graphics pipeline.
     */
    virtual void unbind() = 0;

    /**
     * Checks whether all internal resources are valid.
     *
     * @return True if the geometry buffer is valid, otherwise false.
     */
    virtual bool isValid() const = 0;

    /**
     * Gets the width of the geometry buffer attachments.
     *
     * @return Buffer width in pixels.
     */
    virtual UInt32 getWidth() const = 0;

    /**
     * Gets the height of the geometry buffer attachments.
     *
     * @return Buffer height in pixels.
     */
    virtual UInt32 getHeight() const = 0;

    /**
     * Resizes all geometry buffer attachments.
     *
     * @param width New buffer width in pixels.
     * @param height New buffer height in pixels.
     */
    virtual void resize(UInt32 width, UInt32 height) = 0;

    /**
     * Gets the world-space position texture attachment.
     *
     * @return Const reference to the position texture.
     */
    virtual const ITexture& getPosition() const = 0;

    /**
     * Gets the normal and roughness texture attachment.
     *
     * @return Const reference to the normal/roughness texture.
     */
    virtual const ITexture& getNormalRoughness() const = 0;

    /**
    * Gets the albedo and alpha texture attachment.
    *
    * @return Const reference to the albedo/alpha texture.
    */
    virtual const ITexture& getAlbedoAlpha() const = 0;

    /**
     * Gets the material parameter texture attachment.
     *
     * @return Const reference to the material parameters texture.
     */
    virtual const ITexture& getMaterialParameters() const = 0;

  protected:
    IGBuffer() = default;
  };
}
