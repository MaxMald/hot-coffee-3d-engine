#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Texture;

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
     * Resizes all geometry buffer attachments.
     *
     * @param width New buffer width in pixels.
     * @param height New buffer height in pixels.
     */
    virtual void resize(UInt32 width, UInt32 height) = 0;

    /**
     * Gets the world-space position texture attachment.
     *
     * @return Shared pointer to the position texture.
     */
    virtual SharedPtr<Texture> getPosition() const = 0;

    /**
     * Gets the albedo and alpha texture attachment.
     *
     * @return Shared pointer to the albedo/alpha texture.
     */
    virtual SharedPtr<Texture> getAlbedoAlpha() const = 0;

    /**
     * Gets the normal and roughness texture attachment.
     *
     * @return Shared pointer to the normal/roughness texture.
     */
    virtual SharedPtr<Texture> getNormalRoughness() const = 0;

    /**
     * Gets the material parameter texture attachment.
     *
     * @return Shared pointer to the material parameters texture.
     */
    virtual SharedPtr<Texture> getMaterialParameters() const = 0;

  protected:
    IGBuffer() = default;
  };
}
